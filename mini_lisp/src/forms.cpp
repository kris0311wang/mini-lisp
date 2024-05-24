//
// Created by 王愉博 on 24-5-23.
//
#include"forms.h"
#include"error.h"
#include"value.h"
#include"eval_env.h"

const std::unordered_map<std::string, SpecialFormType *> SPECIAL_FORMS{
        {"define", &defineForm},
        {"if", &ifForm},
        {"and",&andForm},
        {"or",&orForm},
        {"lambda", &lambdaForm},
        {"quote", &quoteForm},
//    {"set!", &setForm},
};

ValuePtr defineForm(const std::vector<ValuePtr> &args, EvalEnv &env) {
    if (args.size() != 2) {
        throw LispError("define form must have 2 arguments");
    }
    if (args[0]->isSymbol()) {
        env.defineBinding(*args[0]->asSymbol(), env.eval(args[1]));
        return std::make_shared<NilValue>();
    } else if(args[0]->isPair()&&args[1]->isPair()) {//lambda形式的定义
        auto paramsVector = args[0]->toVector();
        auto bodyVector = args[1]->toVector();
        if(!paramsVector[0]->isSymbol()){
            throw LispError("lambda form's first argument must be a list of symbols.");
        }
        auto name=*paramsVector[0]->asSymbol();//获取lambda名字
        paramsVector.erase(paramsVector.begin());//删除lambda名字
        std::vector<std::string> paramNamesStr;
        for(const auto& i:paramsVector){
            if(auto paramName=i->asSymbol()){
                paramNamesStr.push_back(*paramName);
            }else{
                throw LispError("lambda form's first argument must be a list of symbols.");
            }
        }
        env.defineBinding(name,std::make_shared<LambdaValue>(paramNamesStr,bodyVector,env.createChild()));
        return std::make_shared<NilValue>();
    }else {
            throw LispError("unimplemented define form");
        }
}


ValuePtr quoteForm(const std::vector<ValuePtr> &params, EvalEnv &env) {
    if(params.size()!=1){//quote特殊形式只能有一个参数
        throw LispError("quote form must have exactly one argument.");
    }
    return params[0]->toQuote();
}

ValuePtr ifForm(const std::vector<ValuePtr> &params, EvalEnv &env) {
    if (params.size() != 3) {  // if特殊形式只能有三个参数
        throw LispError("if form must have exactly three arguments.");
    }else if(*env.eval(params[0])->asBool() ){//如果第一个参数为真
        return env.eval(params[1]);//返回第二个参数
    }else{
        return env.eval(params[2]);//返回第三个参数
    }
}

ValuePtr andForm(const std::vector<ValuePtr> &params, EvalEnv &env) {

    if (params.empty()) {
        return std::make_shared<BooleanValue>(true);
    }

    ValuePtr andresult;
    for(const auto& i:params){
        andresult=env.eval(i);
        if(!*andresult->asBool()){//如果有一个参数为假
            return andresult;//返回假
        }
    }
    return andresult;
}

ValuePtr orForm(const std::vector<ValuePtr> &params, EvalEnv &env) {
    if(params.empty()){
        return std::make_shared<BooleanValue>(false);
    }
    for(const auto& i:params){
        auto orResult = env.eval(i);
        if(*orResult->asBool()){//如果有一个参数为真
            return orResult;
        }
    }
    return env.eval(params[params.size()-1]);
}

ValuePtr lambdaForm(const std::vector<ValuePtr>& params, EvalEnv& env){
    if(params.size()!=2){
        throw LispError("lambda form must have exactly two arguments.");
    }
    auto paramNamesValuePtrVector=params[0]->toVector();
    std::vector<std::string> paramNamesStrVector;
    for(const auto& i:paramNamesValuePtrVector){
        if(auto name=i->asSymbol()){
            paramNamesStrVector.push_back(*name);
        }else{
            throw LispError("lambda form's first argument must be a list of symbols.");
        }
    }
    return std::make_shared<LambdaValue>(paramNamesStrVector, params[1]->toVector(),env.createChild());
}
