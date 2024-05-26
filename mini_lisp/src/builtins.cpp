//
// Created by 王愉博 on 2024/5/15.
//
#include"value.h"
#include"builtins.h"
#include"error.h"
#include"eval_env.h"

std::unordered_map<std::string, std::shared_ptr<BuiltinProcValue>> builtin_funcs{
        {"+",         std::make_shared<BuiltinProcValue>("+", add)},
        {"print",     std::make_shared<BuiltinProcValue>("print", print)},
        {"-",         std::make_shared<BuiltinProcValue>("-", sub)},
        {">",         std::make_shared<BuiltinProcValue>(">", greaterThan)},
        {"<",         std::make_shared<BuiltinProcValue>("<", lessThan)},
        {"length",    std::make_shared<BuiltinProcValue>("length", length)},
        {"car",       std::make_shared<BuiltinProcValue>("car", car)},
        {"cdr",       std::make_shared<BuiltinProcValue>("cdr", cdr)},
        {"*",         std::make_shared<BuiltinProcValue>("*", multiply)},
        {"apply",     std::make_shared<BuiltinProcValue>("apply", apply)},
        {"display",   std::make_shared<BuiltinProcValue>("display", display)},
        {"displayln", std::make_shared<BuiltinProcValue>("displayln", displayln)},
        {"error",     std::make_shared<BuiltinProcValue>("error", error)},
        {"eval",      std::make_shared<BuiltinProcValue>("eval", eval)},
        {"cons",      std::make_shared<BuiltinProcValue>("cons", cons)},
        {"exit",      std::make_shared<BuiltinProcValue>("exit", builtinExit)},
        {"newline",   std::make_shared<BuiltinProcValue>("newline", newline)},
        {"atom?", std::make_shared<BuiltinProcValue>("atom?", atomCheck)},
        {"boolean?", std::make_shared<BuiltinProcValue>("boolean?", booleanCheck)},
        {"integer?", std::make_shared<BuiltinProcValue>("integer?", intergerCheck)},
        {"number?",std::make_shared<BuiltinProcValue>("number?", numberCheck)},
        {"null?",std::make_shared<BuiltinProcValue>("null?", nullCheck)},
        {"list?",std::make_shared<BuiltinProcValue>("list?", listCheck)},
        {"pair?",std::make_shared<BuiltinProcValue>("pair?", pairCheck)}


};  // 内建函数的map

ValuePtr add(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {//add函数的实现:输入是一个ValuePtr的vector，输出是一个ValuePtr
    double result = 0;
    for (const auto &i: params) {
        if (i->isNum()) {
            result += *i->asNumber();
        } else {
            throw LispError("add: arguments must be numbers.");
        }
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr print(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {//print函数的实现:输入是一个ValuePtr的vector，输出空表
    for (const auto &i: params) {
        std::cout << i->toString() << " ";
    }
    std::cout << std::endl;
    return std::make_shared<NilValue>();
}


ValuePtr sub(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    if (params.size() != 2) {
        throw LispError("sub: arguments must be 2 numbers.");
    }
    if (!(params[0]->isNum() && params[1]->isNum())) {
        throw LispError("sub: arguments must be numbers.");
    }
    return std::make_shared<NumericValue>(*params[0]->asNumber() - *params[1]->asNumber());
}

ValuePtr greaterThan(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    if (params.size() != 2) {
        throw LispError("greaterThan: arguments must be 2 numbers.");
    }
    if (!(params[0]->isNum() && params[1]->isNum())) {
        throw LispError("greaterThan: arguments must be numbers.");
    }
    return std::make_shared<BooleanValue>(*params[0]->asNumber() > *params[1]->asNumber());
}

ValuePtr lessThan(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    if (params.size() != 2) {
        throw LispError("lessThan: arguments must be 2 numbers.");
    }
    if (!(params[0]->isNum() && params[1]->isNum())) {
        throw LispError("lessThan: arguments must be numbers.");
    }
    return std::make_shared<BooleanValue>(*params[0]->asNumber() < *params[1]->asNumber());
}

ValuePtr length(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    if (params.size() != 1) {
        throw LispError("length: arguments must be 1 list.");
    }
    return std::make_shared<NumericValue>(params[0]->toVector().size());
}

ValuePtr car(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    if (params.size() != 1) {
        throw LispError("car: arguments must be 1 list.");
    }
    if (!params[0]->isPair()) {
        throw LispError("car: arguments must be a list.");
    }
    return std::static_pointer_cast<PairValue>(params[0])->getCar();
}

ValuePtr cdr(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    if (params.size() != 1) {
        throw LispError("cdr: arguments must be 1 list.");
    }
    if (!params[0]->isPair()) {
        throw LispError("cdr: arguments must be a list.");
    }
    return std::static_pointer_cast<PairValue>(params[0])->getCdr();
}

ValuePtr multiply(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    double result = 1;
    for (const auto &i: params) {
        if (i->isNum()) {
            result *= *i->asNumber();
        } else {
            throw LispError("add: arguments must be numbers.");
        }
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr apply(const std::vector<ValuePtr> &params, EvalEnv &env) {
    if (params.size() != 2) {
        throw LispError("apply: arguments must be 2.");
    }
    return EvalEnv::apply(params[0], params[1]->toVector(), env);
}

ValuePtr display(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {//对于stringvalue输出内容，对于其他输出string表示
    if (params.size() != 1) {
        throw LispError("display: arguments must be 1.");
    }
    if (params[0]->isString()) {
        std::cout << params[0]->internalToString();
    } else {
        std::cout << params[0]->toString();
    }
    return std::make_shared<NilValue>();
}

ValuePtr displayln(const std::vector<ValuePtr> &params, EvalEnv &env) {//display加上换行
    display(params, env);
    std::cout << std::endl;
    return std::make_shared<NilValue>();
}

ValuePtr error(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    if(params.empty()){
        throw LispError("default error message");
    }else if(params.size() != 1){
        throw LispError("error: arguments more than 1.");
    }
    throw LispError(params[0]->toString());
}

ValuePtr eval(const std::vector<ValuePtr> &params, EvalEnv &env) {
    return env.eval(params);
}

ValuePtr cons(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    if (params.size() != 2) {
        throw LispError("cons: arguments must be 2.");
    }
    return std::make_shared<PairValue>(params[0], params[1]);
}

ValuePtr builtinExit(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv& env){
    if(params.empty()){
        exit(0);
    }
    if(params.size() == 1){
        if(params[0]->isInt()){
            exit(*params[0]->asNumber());  // NOLINT(*-narrowing-conversions)
        }
    }
    throw LispError("exit: arguments must be an integer");
}

ValuePtr newline(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv& env){
    if(!params.empty()){
        throw LispError("newline: arguments must be 0.");
    }
    std::cout << std::endl;
    return std::make_shared<NilValue>();
}

ValuePtr atomCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env){
    if(params.size() != 1){
        throw LispError("atom?: arguments must be 1.");
    }
    return std::make_shared<BooleanValue>(params[0]->isSelfEvaluating()||params[0]->isNil()||params[0]->isSymbol());
}

ValuePtr booleanCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    if(params.size() != 1){
        throw LispError("boolean?: arguments must be 1.");
    }
    return std::make_shared<BooleanValue>(params[0]->isBool());
}

ValuePtr intergerCheck(const std::vector<ValuePtr> &params,[[maybe_unused]] EvalEnv &env){
    if(params.size() != 1){
        throw LispError("integer?: arguments must be 1.");
    }
    return std::make_shared<BooleanValue>(params[0]->isInt());
}

ValuePtr listCheck(const std::vector<ValuePtr> &params,[[maybe_unused]] EvalEnv &env){
    if(params.size() != 1){
        throw LispError("list?: arguments must be 1.");
    }
    return std::make_shared<BooleanValue>(params[0]->isList());
}

ValuePtr numberCheck(const std::vector<ValuePtr> &params,[[maybe_unused]] EvalEnv &env){
    if(params.size() != 1){
        throw LispError("number?: arguments must be 1.");
    }
    return std::make_shared<BooleanValue>(params[0]->isNum());
}

ValuePtr nullCheck(const std::vector<ValuePtr> &params,[[maybe_unused]] EvalEnv &env){
    if(params.size() != 1){
        throw LispError("null?: arguments must be 1.");
    }
    return std::make_shared<BooleanValue>(params[0]->isNil());
}

ValuePtr pairCheck(const std::vector<ValuePtr> &params,[[maybe_unused]] EvalEnv &env){
    if(params.size()!=1){
        throw LispError("pair?: arguments muse be 1");
    }
    return std::make_shared<BooleanValue>(params[0]->isPair());
}
