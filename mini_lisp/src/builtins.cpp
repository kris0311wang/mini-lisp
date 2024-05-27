#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
//
// Created by 王愉博 on 2024/5/15.
//
#include"value.h"
#include"builtins.h"
#include"error.h"
#include"eval_env.h"

std::unordered_map<std::string, std::shared_ptr<BuiltinProcValue>> builtin_funcs{
        {"+",          std::make_shared<BuiltinProcValue>("+", add)},
        {"print",      std::make_shared<BuiltinProcValue>("print", print)},
        {"-",          std::make_shared<BuiltinProcValue>("-", sub)},
        {">",          std::make_shared<BuiltinProcValue>(">", greaterThan)},
        {"<",          std::make_shared<BuiltinProcValue>("<", lessThan)},
        {"length",     std::make_shared<BuiltinProcValue>("length", length)},
        {"car",        std::make_shared<BuiltinProcValue>("car", car)},
        {"cdr",        std::make_shared<BuiltinProcValue>("cdr", cdr)},
        {"*",          std::make_shared<BuiltinProcValue>("*", multiply)},
        {"apply",      std::make_shared<BuiltinProcValue>("apply", apply)},
        {"display",    std::make_shared<BuiltinProcValue>("display", display)},
        {"displayln",  std::make_shared<BuiltinProcValue>("displayln", displayln)},
        {"error",      std::make_shared<BuiltinProcValue>("error", error)},
        {"eval",       std::make_shared<BuiltinProcValue>("eval", eval)},
        {"cons",       std::make_shared<BuiltinProcValue>("cons", cons)},
        {"exit",       std::make_shared<BuiltinProcValue>("exit", builtinExit)},
        {"newline",    std::make_shared<BuiltinProcValue>("newline", newline)},
        {"atom?",      std::make_shared<BuiltinProcValue>("atom?", atomCheck)},
        {"boolean?",   std::make_shared<BuiltinProcValue>("boolean?", booleanCheck)},
        {"integer?",   std::make_shared<BuiltinProcValue>("integer?", intergerCheck)},
        {"number?",    std::make_shared<BuiltinProcValue>("number?", numberCheck)},
        {"null?",      std::make_shared<BuiltinProcValue>("null?", nullCheck)},
        {"list?",      std::make_shared<BuiltinProcValue>("list?", listCheck)},
        {"pair?",      std::make_shared<BuiltinProcValue>("pair?", pairCheck)},
        {"procedure?", std::make_shared<BuiltinProcValue>("procedure?", procedureCheck)},
        {"string?",    std::make_shared<BuiltinProcValue>("string?", stringCheck)},
        {"symbol?",    std::make_shared<BuiltinProcValue>("symbol?", symbolCheck)},
        {"append",     std::make_shared<BuiltinProcValue>("append", append)},
        {"list",       std::make_shared<BuiltinProcValue>("list", list)},
        {"map",        std::make_shared<BuiltinProcValue>("map", map)},
        {"reduce",     std::make_shared<BuiltinProcValue>("reduce", reduce)},
        {"/",          std::make_shared<BuiltinProcValue>("/", divide)},
        {"expt",       std::make_shared<BuiltinProcValue>("expt", expt)}
};  // 内建函数的map

ValuePtr
add(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {//add函数的实现:输入是一个ValuePtr的vector，输出是一个ValuePtr
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

ValuePtr
print(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {//print函数的实现:输入是一个ValuePtr的vector，输出空表
    for (const auto &i: params) {
        std::cout << i->toString() << " ";
    }
    std::cout << std::endl;
    return std::make_shared<NilValue>();
}


ValuePtr sub(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    if (params.size() != 2 && params.size() != 1) {
        throw LispError("sub: arguments must be 2 numbers.");
    }
    if (params.size() == 1) {
        if (!params[0]->isNum()) {
            throw LispError("sub: arguments must be numbers.");
        }
        return std::make_shared<NumericValue>(-*params[0]->asNumber());

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
    if (params.size() != 1 || !params[0]->isList()) {
        throw LispError("length: arguments must be a list.");
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
    if (params.empty()) {
        throw LispError("default error message");
    } else if (params.size() != 1) {
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

ValuePtr builtinExit(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    if (params.empty()) {
        exit(0);
    }
    if (params.size() == 1) {
        if (params[0]->isInt()) {
            exit(*params[0]->asNumber());  // NOLINT(*-narrowing-conversions)
        }
    }
    throw LispError("exit: arguments must be an integer");
}

ValuePtr newline(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    if (!params.empty()) {
        throw LispError("newline: arguments must be 0.");
    }
    std::cout << std::endl;
    return std::make_shared<NilValue>();
}

ValuePtr atomCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    if (params.size() != 1) {
        throw LispError("atom?: arguments must be 1.");
    }
    return std::make_shared<BooleanValue>(params[0]->isSelfEvaluating() || params[0]->isNil() || params[0]->isSymbol());
}

ValuePtr booleanCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    if (params.size() != 1) {
        throw LispError("boolean?: arguments must be 1.");
    }
    return std::make_shared<BooleanValue>(params[0]->isBool());
}

ValuePtr intergerCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    if (params.size() != 1) {
        throw LispError("integer?: arguments must be 1.");
    }
    return std::make_shared<BooleanValue>(params[0]->isInt());
}

ValuePtr listCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    if (params.size() != 1) {
        throw LispError("list?: arguments must be 1.");
    }
    return std::make_shared<BooleanValue>(params[0]->isList());
}

ValuePtr numberCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    if (params.size() != 1) {
        throw LispError("number?: arguments must be 1.");
    }
    return std::make_shared<BooleanValue>(params[0]->isNum());
}

ValuePtr nullCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    if (params.size() != 1) {
        throw LispError("null?: arguments must be 1.");
    }
    return std::make_shared<BooleanValue>(params[0]->isNil());
}

ValuePtr pairCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    if (params.size() != 1) {
        throw LispError("pair?: arguments muse be 1");
    }
    return std::make_shared<BooleanValue>(params[0]->isPair());
}

ValuePtr procedureCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    if (params.size() != 1) {
        throw LispError("builtin?: arguments muse be 1");
    }
    return std::make_shared<BooleanValue>(params[0]->isBuiltin() || params[0]->isLambda());
}

ValuePtr stringCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    if (params.size() != 1) {
        throw LispError("builtin?: arguments muse be 1");
    }
    return std::make_shared<BooleanValue>(params[0]->isString());
}

ValuePtr symbolCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    if (params.size() != 1) {
        throw LispError("builtin?: arguments muse be 1");
    }
    return std::make_shared<BooleanValue>(params[0]->isSymbol());
}

ValuePtr append(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    if (params.empty()) {//如果参数为空,返回空表
        return std::make_shared<NilValue>();
    }
    if (!params[0]->isList()) {//如果第一个参数不是list,抛出异常
        throw LispError("append: arguments must be lists.");
    }
    auto result = std::static_pointer_cast<PairValue>(params[0]);
    for (int i = 1; i < params.size(); i++) {
        result->append(params[i]);//将params[i]加入到result的最后
    }
    return result;
}

ValuePtr list(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    return std::make_shared<PairValue>(params);
}

ValuePtr map(const std::vector<ValuePtr> &params, EvalEnv &env) {
    if (params.size() != 2) {
        throw LispError("map: arguments must be 2.");
    }
    if (!params[0]->isBuiltin()) {
        throw LispError("map: first argument must be a procedure.");
    }
    if (!params[1]->isList()) {
        throw LispError("map: second argument must be a list.");
    }
    const auto &proc = params[0];
    auto list = params[1]->toVector();
    std::vector<ValuePtr> result;
    result.reserve(list.size());//预留空间(clang-tidy)
    for (const auto &i: list) {
        result.push_back(EvalEnv::apply(proc, {i}, env));
    }
    return std::make_shared<PairValue>(result);
}

ValuePtr reduce(const std::vector<ValuePtr> &params, EvalEnv &env) {
    if (params.size() != 2) {
        throw LispError("reduce: arguments must be 2.");
    }
    if (!params[0]->isBuiltin()) {
        throw LispError("reduce: first argument must be a procedure.");
    }
    if (!params[1]->isList()) {
        throw LispError("reduce: second argument must be a list.");
    }
    const auto &proc = params[0];
    auto list = std::static_pointer_cast<PairValue>(params[1]);
    if (list->getCdr()->isNil()) {
        return list->getCar();
    }
    auto result = EvalEnv::apply(proc, {list->getCar(), reduce({proc, list->getCdr()}, env)}, env);
    return result;
}

ValuePtr divide(const std::vector<ValuePtr> &params, EvalEnv &env) {
    if(params.size()!=2){
        throw LispError("divide: arguments must be 2 numbers.");
    }
    if(!(params[0]->isNum()&&params[1]->isNum())){
        throw LispError("divide: arguments must be numbers.");
    }
    if(*params[1]->asNumber()==0){
        throw LispError("divide: divisor can't be zero.");
    }
    return std::make_shared<NumericValue>(*params[0]->asNumber()/ *params[1]->asNumber());
}

ValuePtr expt(const std::vector<ValuePtr> &params, EvalEnv &env){
    if(params.size()!=2){
        throw LispError("expt: arguments must be 2 numbers.");
    }
    if(!(params[0]->isNum()&&params[1]->isNum())){
        throw LispError("expt: arguments must be numbers.");
    }
    auto x=*params[0]->asNumber();
    auto y=*params[1]->asNumber();
    if(x==0&&y==0){
        throw LispError("expt: 0^0 is undefined.");
    }
    return std::make_shared<NumericValue>(std::pow(x,y));
}


#pragma clang diagnostic pop