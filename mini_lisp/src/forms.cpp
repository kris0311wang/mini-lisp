//
// Created by 王愉博 on 24-5-23.
//
#include"forms.h"
#include"error.h"
#include"value.h"
#include"eval_env.h"
#include"builtins.h"
const std::unordered_map<std::string, SpecialFormType *> SPECIAL_FORMS{
        {"define", &defineForm},
        {"if",     &ifForm},
        {"and",    &andForm},
        {"or",     &orForm},
        {"lambda", &lambdaForm},
        {"quote",  &quoteForm},
        {"cond",&condForm},
        {"begin",&beginForm},
        {"let",&letForm},
        {"quasiquote",&quasiquoteForm},
        {"quasiquote",&quasiquoteForm},
//    {"set!", &setForm},
};
void checkMinSize(const std::vector<ValuePtr> &params, int size, const std::string &name);

void checkExactSize(const std::vector<ValuePtr> &params, int size, const std::string &name);

void checkMaxSize(const std::vector<ValuePtr> &params, int size, const std::string &name);

void throwTypeError(const std::string &name, const std::string &expectedType);

ValuePtr singleElementToAtom(const ValuePtr &value) {
    if(value->isList()){
        auto list=value->toVector();
        if(list.size()==1){
            return list[0];
        }
        throw LispError("singleElementToAtom: list size is not 1");
    }
    return value;
}

ValuePtr defineForm(const std::vector<ValuePtr> &args, EvalEnv &env) {
    if (args[0]->isSymbol()) {//普通形式的定义
        env.defineBinding(*args[0]->asSymbol(), env.eval(args[1]));
        return std::make_shared<NilValue>();
    } else if (args[0]->isPair() ) {//lambda形式的定义
        auto paramsVector = args[0]->toVector();//获取lambda参数
        auto bodyVector = std::vector<ValuePtr>(args.begin() + 1, args.end());//获取lambda体
        if (!paramsVector[0]->isSymbol()) {
            throw LispError("lambda form's first argument must be a list of symbols.");
        }
        auto name = *paramsVector[0]->asSymbol();//获取lambda名字
        paramsVector.erase(paramsVector.begin());//删除lambda名字
        std::vector<std::string> paramNamesStr;
        for (const auto &i: paramsVector) {
            if (auto paramName = i->asSymbol()) {
                paramNamesStr.push_back(*paramName);
            } else {
                throw LispError("lambda form's first argument must be a list of symbols.");
            }
        }
        env.defineBinding(name, std::make_shared<LambdaValue>(paramNamesStr, bodyVector, env.createChild()));
        return std::make_shared<NilValue>();
    } else {
        throw LispError("unimplemented define form");
    }
}


ValuePtr quoteForm(const std::vector<ValuePtr> &params, EvalEnv &env) {
    checkExactSize(params, 1, "quote");//检查参数个数是否为1
    return params[0]->toQuote();
}

ValuePtr ifForm(const std::vector<ValuePtr> &params, EvalEnv &env) {
    checkExactSize(params, 3, "if");//检查参数个数是否为3
    if (*env.eval(params[0])->asBool()) {//如果第一个参数为真
        return env.eval(params[1]);//返回第二个参数
    } else {
        return env.eval(params[2]);//返回第三个参数
    }
}

ValuePtr andForm(const std::vector<ValuePtr> &params, EvalEnv &env) {
    if (params.empty()) {
        return std::make_shared<BooleanValue>(true);
    }

    ValuePtr andresult;
    for (const auto &i: params) {
        andresult = env.eval(i);
        if (!*andresult->asBool()) {//如果有一个参数为假
            return andresult;//返回假
        }
    }
    return andresult;
}

ValuePtr orForm(const std::vector<ValuePtr> &params, EvalEnv &env) {
    if (params.empty()) {
        return std::make_shared<BooleanValue>(false);
    }
    for (const auto &i: params) {
        auto orResult = env.eval(i);
        if (*orResult->asBool()) {//如果有一个参数为真
            return orResult;
        }
    }
    return env.eval(params[params.size() - 1]);
}

ValuePtr lambdaForm(const std::vector<ValuePtr> &params, EvalEnv &env) {
    checkMinSize(params, 2, "lambda");//检查参数个数是否为2
    auto paramNamesValuePtrVector = params[0]->toVector();
    std::vector<std::string> paramNamesStrVector;
    for (const auto &i: paramNamesValuePtrVector) {
        if (auto name = i->asSymbol()) {
            paramNamesStrVector.push_back(*name);
        } else {
            throw LispError("lambda form's first argument must be a list of symbols.");
        }
    }
    return std::make_shared<LambdaValue>(paramNamesStrVector, std::vector<ValuePtr>(params.begin()+1,params.end()), env.shared_from_this());
}

ValuePtr condForm(const std::vector<ValuePtr> &params, EvalEnv &env) {
    checkMinSize(params, 1, "cond");
    for (const auto &i: params) {
        if(!i->isPair()){
            throw LispError("cond form's arguments must be pairs.");
        }
        auto pairExpression=std::static_pointer_cast<PairValue>(i);
        auto condition=pairExpression->getCar();
        auto condResult=env.eval(condition);//计算条件
        if(*condition->asSymbol()=="else"||*condResult->asBool()){
            auto expressions = pairExpression->getCdr()->toVector();
            ValuePtr result;
            if(expressions.empty()){//如果没有表达式,返回计算条件的结果
                return condResult;
            }
            for (const auto & expression : expressions) {
                result = env.eval(expression);//计算每个表达式
            }
            return result;//返回最后一个表达式的结果
        }
    }
    return std::make_shared<NilValue>();//如果没有条件为真,返回空表
}

ValuePtr beginForm(const std::vector<ValuePtr> &params, EvalEnv &env) {
    checkMinSize(params, 1, "begin");//检查参数个数是否大于等于1
    //其余移植lambda的body计算过程
    if(params.size() == 1) {
        return env.eval(params[0]);
    }
    ValuePtr result;
    std::vector<ValuePtr> resultVector;
    for(int i=0;i<params.size()-1;i++){
        result=env.eval(params[i]);
    }
    return env.eval(params[params.size()-1]);
}

ValuePtr letForm(const std::vector<ValuePtr> &params, EvalEnv &env) {//(let 绑定 表达式)
    checkMinSize(params, 2, "let");//检查参数个数是否大于等于2
    auto bindings=std::static_pointer_cast<PairValue>(params[0])->toVector();//获取绑定
    auto expressions=std::vector<ValuePtr>(params.begin()+1,params.end());//获取表达式
    auto newEnv=env.createChild();//创建新环境
    std::vector<std::string> paramNamesStrVector;
    std::vector<ValuePtr> paramValuesVector;
    for(const auto &i:bindings){//对每个绑定进行处理
        if(!i->isPair()){
            throw LispError("let form's first argument must be a list of pairs.");
        }
        auto bindingVector=i->toVector();
        checkExactSize(bindingVector,2,"let Form's binding pair");
        auto name=bindingVector[0]->asSymbol();//获取名字
        auto value=bindingVector[1];//获取值
        if(!name) {
            throwTypeError("let Form's binding pair's first element", "symbol");
        }
        newEnv->defineBinding(*name,value);//将值绑定到名字
        paramNamesStrVector.push_back(*name);
        paramValuesVector.push_back(newEnv->eval(value));//初始值
    }
    auto letLambda=std::make_shared<LambdaValue>(paramNamesStrVector,expressions,newEnv);
    return letLambda->apply(paramValuesVector);
}

ValuePtr quasiquoteForm(const std::vector<ValuePtr> &params,EvalEnv& env){
    checkExactSize(params,1,"quasiquote");
    if(params[0]->isPair()){
        auto pairvalue=std::static_pointer_cast<PairValue>(params[0]);
        if(auto symbolHead =pairvalue->getCar()->asSymbol()){
            if(*symbolHead=="unquote"){//如果是unquote开头的对子
                auto result= env.eval(pairvalue->getCdr()->toVector()[0]);
                return result->toQuote();
            }
        }
        return std::make_shared<PairValue>(quasiquoteForm({pairvalue->getCar()},env),quasiquoteForm({pairvalue->getCdr()},env));//递归调用quasiquote
    }
    return params[0]->toQuote();//其他类型不可能为unquote，直接返回quote
    throw LispError("quasiquote unimplemented.");
}

ValuePtr unquoteForm(const std::vector<ValuePtr> &params,EvalEnv& env){
    checkExactSize(params,1,"unquote");

}