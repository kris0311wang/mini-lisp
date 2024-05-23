//
// Created by 王愉博 on 24-5-23.
//
#include"forms.h"
#include"error.h"

const std::unordered_map<std::string, SpecialFormType *> SPECIAL_FORMS{
        {"define", &defineForm},
        {"if", &ifForm},
        {"and",&andForm},
        {"or",&orForm},
//        {"lambda", &lambdaForm},
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
    } else {
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
