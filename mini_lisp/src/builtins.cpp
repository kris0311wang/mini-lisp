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
        {"expt",       std::make_shared<BuiltinProcValue>("expt", expt)},
        {"quotient",   std::make_shared<BuiltinProcValue>("quotient", quotient)},
        {"remainder", std::make_shared<BuiltinProcValue>("remainder", ValueRemainder)},
        {"modulo",     std::make_shared<BuiltinProcValue>("modulo", modulo)},
        {"equal?", std::make_shared<BuiltinProcValue>("equal?", equalCheck)},
        {"eq?",   std::make_shared<BuiltinProcValue>("eq?", eqCheck)},
        {"not",   std::make_shared<BuiltinProcValue>("not", notCheck)},
        {"=",     std::make_shared<BuiltinProcValue>("=", equalSignCheck)},
        {">=",    std::make_shared<BuiltinProcValue>(">=", greaterThanOrEqual)},
        {"<=",    std::make_shared<BuiltinProcValue>("<=", lessThanOrEqual)},
        {"even?", std::make_shared<BuiltinProcValue>("even?", evenCheck)},
        {"odd?",  std::make_shared<BuiltinProcValue>("odd?", oddCheck)},
        {"zero?",std::make_shared<BuiltinProcValue>("zero?",zeroCheck)},
        {"filter",std::make_shared<BuiltinProcValue>("filter",filter)}
};  // 内建函数的map

void checkExactSize(const std::vector<ValuePtr> &params, int size, const std::string &name) {//检查参数个数是否等于size
    if (params.size() != size) {
        throw LispError(name + ": arguments must be " + std::to_string(size) + ", but got " + std::to_string(params.size()) + ".");
    }
}

void throwTypeError(const std::string &name,const std::string& type) {//抛出类型错误
    throw LispError(name + ": arguments must be " + type + ".");
}

void checkMinSize(const std::vector<ValuePtr> &params, int size, const std::string &name) {//检查参数个数是否大于等于size
    if (params.size() < size) {
        throw LispError(name + ": arguments must be at least " + std::to_string(size) + "，but got " + std::to_string(params.size()) + ".");
    }
}

void checkMaxSize(const std::vector<ValuePtr> &params, int size, const std::string &name) {//检查参数个数是否小于等于size
    if (params.size() > size) {
        throw LispError(name + ": arguments must be at most " + std::to_string(size) + "，but got " + std::to_string(params.size()) + ".");
    }
}

ValuePtr add(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {//add函数的实现:输入是一个ValuePtr的vector，输出是一个ValuePtr
    double result = 0;
    for (const auto &i: params) {
        if (auto num = i->asNumber()){
            result += *num;
        } else {
            throwTypeError("add","numbers");
        }
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr print(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {//print函数的实现:输入是一个ValuePtr的vector，输出空表
    checkExactSize(params, 1, "print");
    std::cout << params[0]->toString()<<std::endl;
    return std::make_shared<NilValue>();
}


ValuePtr sub(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    checkMaxSize(params, 2, "sub");
    if (params.size() == 1) {
        if (!params[0]->isNum()) {
            throwTypeError("sub","numbers");
        }
        return std::make_shared<NumericValue>(-*params[0]->asNumber());
    }
    return std::make_shared<NumericValue>(*params[0]->asNumber() - *params[1]->asNumber());
}

ValuePtr greaterThan(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    checkExactSize(params, 2, ">");//检查参数个数是否等于2
    auto x=params[0]->asNumber();
    auto y=params[1]->asNumber();
    if(x && y){
        return std::make_shared<BooleanValue>(*params[0]->asNumber() > *params[1]->asNumber());
    }
    throwTypeError(">","numbers");
}

ValuePtr lessThan(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    checkExactSize(params, 2, "<");
    return greaterThan({params[1], params[0]}, env);
}

ValuePtr length(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    checkExactSize(params, 1, "length");
    if (!params[0]->isList()) {
        throwTypeError("length","lists");
    }
    return std::make_shared<NumericValue>(params[0]->toVector().size());
}

ValuePtr car(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    if (params.size() != 1) {
        throwTypeError("car","lists");
    }
    if (!params[0]->isPair()) {
        throwTypeError("car","lists");
    }
    return std::static_pointer_cast<PairValue>(params[0])->getCar();
}

ValuePtr cdr(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    checkExactSize(params, 1, "cdr");
    if (!params[0]->isPair()) {
        throwTypeError("cdr","lists");
    }
    return std::static_pointer_cast<PairValue>(params[0])->getCdr();
}

ValuePtr multiply(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    double result = 1;
    for (const auto &i: params) {
        if (auto num=i->asNumber()) {
            result *= *num;
        } else {
            throwTypeError("multiply","numbers");
        }
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr apply(const std::vector<ValuePtr> &params, EvalEnv &env) {
    checkExactSize(params, 2, "apply");
    return EvalEnv::apply(params[0], params[1]->toVector(), env);
}

ValuePtr display(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {//对于stringvalue输出内容，对于其他输出string表示
    checkExactSize(params, 1, "display");
    if (params[0]->isString()) {
        std::cout << std::static_pointer_cast<StringValue>(params[0])->getValue();
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
    checkMaxSize(params, 1, "error");
    if (params.empty()) {
        throw LispError("default error message");
    }
    throw LispError(params[0]->toString());
}

ValuePtr eval(const std::vector<ValuePtr> &params, EvalEnv &env) {
    return env.eval(params);
}

ValuePtr cons(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    checkExactSize(params, 2, "cons");
    return std::make_shared<PairValue>(params[0], params[1]);
}

ValuePtr builtinExit(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    checkMaxSize(params, 1, "exit");
    if (params.empty()) {
        exit(0);
    }
    if (params[0]->isInt()) {
        exit(*params[0]->asNumber());  // NOLINT(*-narrowing-conversions)
    }
    throwTypeError("exit","integers");
}

ValuePtr newline(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    checkExactSize(params, 0, "newline");
    std::cout << std::endl;
    return std::make_shared<NilValue>();
}

ValuePtr atomCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    checkExactSize(params, 1, "atom?");
    return std::make_shared<BooleanValue>(params[0]->isSelfEvaluating() || params[0]->isNil() || params[0]->isSymbol());
}

ValuePtr booleanCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    checkExactSize(params, 1, "boolean?");
    return std::make_shared<BooleanValue>(params[0]->isBool());
}

ValuePtr intergerCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    checkExactSize(params, 1, "integer?");
    return std::make_shared<BooleanValue>(params[0]->isInt());
}

ValuePtr listCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    checkExactSize(params, 1, "list?");
    return std::make_shared<BooleanValue>(params[0]->isList());
}

ValuePtr numberCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    checkExactSize(params, 1, "number?");
    return std::make_shared<BooleanValue>(params[0]->isNum());
}

ValuePtr nullCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    checkExactSize(params, 1, "null?");
    return std::make_shared<BooleanValue>(params[0]->isNil());
}

ValuePtr pairCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    checkExactSize(params, 1, "pair?");
    return std::make_shared<BooleanValue>(params[0]->isPair());
}

ValuePtr procedureCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    checkExactSize(params, 1, "procedure?");
    return std::make_shared<BooleanValue>(params[0]->isBuiltin() || params[0]->isLambda());
}

ValuePtr stringCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    checkExactSize(params, 1, "string?");
    return std::make_shared<BooleanValue>(params[0]->isString());
}

ValuePtr symbolCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    checkExactSize(params, 1, "symbol?");
    return std::make_shared<BooleanValue>(params[0]->isSymbol());
}

ValuePtr append(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    if (params.empty()) {//如果参数为空,返回空表
        return std::make_shared<NilValue>();
    }
    if (!params[0]->isList()) {//如果第一个参数不是list,抛出异常
        throwTypeError("append","lists");
    }
    auto result = std::static_pointer_cast<PairValue>(params[0]);
    for (int i = 1; i < params.size(); i++) {
        result->append(params[i]);//将params[i]加入到result的最后
    }
    return result;
}

ValuePtr list(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env) {
    if(params.empty()){//如果参数为空,返回空表
        return std::make_shared<NilValue>();
    }
    return std::make_shared<PairValue>(params);
}

ValuePtr map(const std::vector<ValuePtr> &params, EvalEnv &env) {
    checkExactSize(params, 2, "map");
    if (!params[0]->isBuiltin()) {
        throw LispError("map: first argument must be a procedure.");
    }
    if (!params[1]->isList()) {
        throw LispError("map: second argument must be a list.");
    }
    const auto &proc = params[0];
    auto list = params[1]->toVector();
    std::vector<ValuePtr> result;
    result.reserve(list.size());//预留空间
    for (const auto &i: list) {
        result.push_back(EvalEnv::apply(proc, {i}, env));
    }
    return std::make_shared<PairValue>(result);
}

ValuePtr reduce(const std::vector<ValuePtr> &params, EvalEnv &env) {
    checkExactSize(params, 2, "reduce");
    if (!params[0]->isBuiltin()) {
        throw LispError("reduce: first argument must be a procedure.");
    }
    if (!params[1]->isList()) {
        throw LispError("reduce: second argument must be a list.");
    }
    const auto &proc = params[0];
    auto list = std::static_pointer_cast<PairValue>(params[1]);
    if (list->getCdr()->isNil()) {//如果只有一个元素,返回这个元素
        return list->getCar();
    }
    auto result = EvalEnv::apply(proc, {list->getCar(), reduce({proc, list->getCdr()}, env)}, env);//递归调用
    return result;
}

ValuePtr divide(const std::vector<ValuePtr> &params, EvalEnv &env) {
    checkExactSize(params, 2, "/");
    if(!(params[0]->isNum()&&params[1]->isNum())){
        throwTypeError("/","numbers");
    }
    if(*params[1]->asNumber()==0){
        throw LispError("divide: divisor can't be zero.");
    }
    return std::make_shared<NumericValue>(*params[0]->asNumber()/ *params[1]->asNumber());
}

ValuePtr expt(const std::vector<ValuePtr> &params, EvalEnv &env){
    checkExactSize(params, 2, "expt");
    auto x=params[0]->asNumber();
    auto y=params[1]->asNumber();
    if(!(x&&y)){
        throwTypeError("expt","numbers");
    }
    if(*x==0&&*y==0){
        throw LispError("expt: 0^0 is undefined.");
    }
    return std::make_shared<NumericValue>(std::pow(*x,*y));
}

ValuePtr quotient(const std::vector<ValuePtr> &params, EvalEnv &env){
    checkExactSize(params, 2, "quotient");
    auto x=params[0]->asNumber();
    auto y=params[1]->asNumber();
    if(!(x&&y)){
        throwTypeError("quotient","numbers");
    }
    if(*params[1]->asNumber()==0){
        throw LispError("quotient: divisor can't be zero.");
    }
    return std::make_shared<NumericValue>(int(*x/ *y));
}

ValuePtr ValueRemainder(const std::vector<ValuePtr> &params, EvalEnv &env){
    checkExactSize(params, 2, "remainder");
    auto x=params[0]->asNumber();
    auto y=params[1]->asNumber();
    if(!(x&&y)){
        throwTypeError("remainder","numbers");
    }
    if(*y==0){
        throw LispError("remainder: divisor can't be zero.");
    }
    if(!(params[0]->isInt() && params[1]->isInt())){
        throwTypeError("remainder","integers");
    }
    return std::make_shared<NumericValue>(int(*x)%int(*y));
}

ValuePtr modulo(const std::vector<ValuePtr> &params, EvalEnv &env) {
    checkExactSize(params, 2, "modulo");
    auto x=params[0]->asNumber();
    auto y=params[1]->asNumber();
    if (!(x&&y)) {
        throwTypeError("modulo","numbers");
    }
    if (*y== 0) {
        throw LispError("modulo: divisor can't be zero.");
    }
    if (!(params[0]->isInt() && params[1]->isInt())) {
        throwTypeError("modulo","integers");
    }
    auto xint=int(*x);
    auto yint=int(*y);
    return std::make_shared<NumericValue>((xint%yint+yint)%yint);
}

ValuePtr equalCheck(const std::vector<ValuePtr> &params, EvalEnv &env) {
    checkExactSize(params, 2, "equal?");
    return std::make_shared<BooleanValue>(*params[0]==*params[1]);
}

ValuePtr eqCheck(const std::vector<ValuePtr> &params, EvalEnv &env) {
    checkExactSize(params, 2, "eq?");
    if(params[0]->isPair()&&params[1]->isPair() ||params[0]->isString()&&params[1]->isString()){
        return std::make_shared<BooleanValue>(params[0]==params[1]);
    }
    return std::make_shared<BooleanValue>(*params[0]==*params[1]);
}

ValuePtr notCheck(const std::vector<ValuePtr> &params, EvalEnv &env) {
    checkExactSize(params, 1, "not");
    return std::make_shared<BooleanValue>(!*params[0]->asBool());
}

ValuePtr equalSignCheck(const std::vector<ValuePtr> &params, EvalEnv &env) {
    checkExactSize(params, 2, "=");
    auto x=params[0]->asNumber();
    auto y=params[1]->asNumber();
    if(!(x&&y)){
        throw LispError("equalSignCheck: arguments must be numbers.");
    }
    return std::make_shared<BooleanValue>(*x==*y);
}

ValuePtr greaterThanOrEqual(const std::vector<ValuePtr> &params, EvalEnv &env) {
    checkExactSize(params, 2, ">=");
    auto x=params[0]->asNumber();
    auto y=params[1]->asNumber();
    if(!(x&&y)){
        throw LispError(">=: arguments must be numbers.");
    }
    return std::make_shared<BooleanValue>(*x>=*y);
}

ValuePtr lessThanOrEqual(const std::vector<ValuePtr> &params, EvalEnv &env) {
    checkExactSize(params, 2, "<=");
    auto x=params[0]->asNumber();
    auto y=params[1]->asNumber();
    if(!(x&&y)){
        throw LispError("<=: arguments must be numbers.");
    }
    return std::make_shared<BooleanValue>(*x<=*y);
}

ValuePtr evenCheck(const std::vector<ValuePtr> &params, EvalEnv &env) {
    checkExactSize(params, 1, "even?");
    if(!params[0]->isInt()){
        throwTypeError("evenCheck","integers");
    }
    return std::make_shared<BooleanValue>(int(*params[0]->asNumber())%2==0);
}

ValuePtr oddCheck(const std::vector<ValuePtr> &params, EvalEnv &env) {
    checkExactSize(params, 1, "odd?");
    if(!params[0]->isInt()){
        throwTypeError("odd?","integers");
    }
    return std::make_shared<BooleanValue>(int(*params[0]->asNumber())%2!=0);
}

ValuePtr zeroCheck(const std::vector<ValuePtr> &params, EvalEnv &env) {
    checkExactSize(params, 1, "zero?");
    auto x=params[0]->asNumber();
    if(!x){
        throwTypeError("zero?","numbers");
    }
    return std::make_shared<BooleanValue>(*x==0);
}

ValuePtr filter(const std::vector<ValuePtr> &params, EvalEnv &env) {
    checkExactSize(params, 2, "filter");
    if(!params[0]->isBuiltin()){
        throw LispError("filter: first argument must be a procedure.");
    }
    if(!params[1]->isList()){
        throw LispError("filter: second argument must be a list.");
    }
    const auto &proc=params[0];
    auto list=params[1]->toVector();
    std::vector<ValuePtr> result;
    result.reserve(list.size());//预留空间(clang-tidy)
    for(const auto &i:list){
        if(*EvalEnv::apply(proc,{i},env)->asBool()){
            result.push_back(i);
        }
    }
    return std::make_shared<PairValue>(result);
}

std::string StringValue::getValue(){
    return value;
}
#pragma clang diagnostic pop