//
// Created by 王愉博 on 2024/5/15.
//
#include"value.h"
#include"builtins.h"
#include"error.h"

std::unordered_map<std::string, std::shared_ptr<BuiltinProcValue>> builtin_funcs{
        {"+", std::make_shared<BuiltinProcValue>("+", add)},
        {"print", std::make_shared<BuiltinProcValue>("print", print)},
        {"-", std::make_shared<BuiltinProcValue>("-", sub)},
        {">", std::make_shared<BuiltinProcValue>(">", greaterThan)},
        {"<", std::make_shared<BuiltinProcValue>("<", lessThan)},
        {"length", std::make_shared<BuiltinProcValue>("length", length)},
        {"car", std::make_shared<BuiltinProcValue>("car", car)},
        {"cdr", std::make_shared<BuiltinProcValue>("cdr", cdr)},
        {"*", std::make_shared<BuiltinProcValue>("*", multiply)}
};  // 内建函数的map
ValuePtr add(const std::vector<ValuePtr> &params) {//add函数的实现:输入是一个ValuePtr的vector，输出是一个ValuePtr
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

ValuePtr print(const std::vector<ValuePtr> &params) {//print函数的实现:输入是一个ValuePtr的vector，输出空表
    for (const auto &i: params) {
        std::cout << i->toString() << " ";
    }
    std::cout << std::endl;
    return std::make_shared<NilValue>();
}


ValuePtr sub(const std::vector<ValuePtr> &params) {
    if(params.size()!=2){
        throw LispError("sub: arguments must be 2 numbers.");
    }
    if(!(params[0]->isNum() && params[1]->isNum())){
        throw LispError("sub: arguments must be numbers.");
    }
    return std::make_shared<NumericValue>(*params[0]->asNumber()-*params[1]->asNumber());
}

ValuePtr greaterThan(const std::vector<ValuePtr> &params) {
    if(params.size()!=2){
        throw LispError("greaterThan: arguments must be 2 numbers.");
    }
    if(!(params[0]->isNum() && params[1]->isNum())){
        throw LispError("greaterThan: arguments must be numbers.");
    }
    return std::make_shared<BooleanValue>(*params[0]->asNumber() > *params[1]->asNumber());
}

ValuePtr lessThan(const std::vector<ValuePtr> &params) {
    if(params.size()!=2){
        throw LispError("lessThan: arguments must be 2 numbers.");
    }
    if(!(params[0]->isNum() && params[1]->isNum())){
        throw LispError("lessThan: arguments must be numbers.");
    }
    return std::make_shared<BooleanValue>(*params[0]->asNumber() < *params[1]->asNumber());
}

ValuePtr length(const std::vector<ValuePtr> &params) {
    if(params.size()!=1){
        throw LispError("length: arguments must be 1 list.");
    }
    return std::make_shared<NumericValue>(params[0]->toVector().size());
}

ValuePtr car(const std::vector<ValuePtr> &params) {
    if(params.size()!=1){
        throw LispError("car: arguments must be 1 list.");
    }
    if(!params[0]->isPair()){
        throw LispError("car: arguments must be a list.");
    }
    return std::static_pointer_cast<PairValue>(params[0])->getCar();
}

ValuePtr cdr(const std::vector<ValuePtr> &params) {
    if(params.size()!=1){
        throw LispError("cdr: arguments must be 1 list.");
    }
    if(!params[0]->isPair()){
        throw LispError("cdr: arguments must be a list.");
    }
    return std::static_pointer_cast<PairValue>(params[0])->getCdr();
}

ValuePtr multiply(const std::vector<ValuePtr> &params) {
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
