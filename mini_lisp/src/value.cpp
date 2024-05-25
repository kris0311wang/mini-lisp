//
// Created by 王愉博 on 24-5-23.
//
#include"value.h"
#include "error.h"
#include <ranges>
#include <sstream>
#include <iomanip>
#include <vector>
#include <optional>
#include"eval_env.h"
std::string BooleanValue::toString() const {
    return value ? "#t" : "#f";
}

bool BooleanValue::getValue() const {
    return value;
}

ValuePtr BooleanValue::toQuote() {
    return shared_from_this();
}

std::string NumericValue::toString() const {
    return isInt() ? std::to_string(static_cast<int>(value)) : std::to_string(value);
}

bool NumericValue::isInt() const {
    return value == static_cast<int>(value);
}

std::string NilValue::toString() const {
    return "()";
}

std::string NilValue::internalToString() const {
    return "";//内部不返回空表两端的括号
}

std::shared_ptr<Value> NilValue::toQuote(){
    return shared_from_this();
}

std::vector<std::shared_ptr<Value>> NilValue::toVector() {
    return {};
}

bool Value::isNil() const {
    return typeid(*this) == typeid(NilValue);
}

bool Value::isSymbol() const {
    return typeid(*this) == typeid(SymbolValue);
}

bool Value::isNum() const {
    return typeid(*this) == typeid(NumericValue);
}

bool Value::isBool() const {
    return typeid(*this) == typeid(BooleanValue);
}

bool Value::isString() const {
    return typeid(*this) == typeid(StringValue);
}

bool Value::isPair() const {
    return typeid(*this) == typeid(PairValue);
}
bool Value::isBuiltin() const {
    return typeid(*this)==typeid(BuiltinProcValue);
}



std::string Value::internalToString() const {
    return toString();
}

bool Value::isSelfEvaluating() const {
    return isNum() || isBool() || isString()|| typeid(*this)==typeid(BuiltinProcValue)||typeid(*this)==typeid(LambdaValue) ;
}

std::vector<std::shared_ptr<Value>> Value::toVector() {
    return {shared_from_this()};
}

std::string PairValue::internalToString() const {
    if (cdr->isNil()) {
        return car->internalToString();
    } else if (!cdr->isPair() && cdr) {
        return car->internalToString() + " . " + cdr->internalToString();
    } else if(cdr) {
        return car->internalToString() + " " + cdr->internalToString();
    }
    throw LispError("PairValue internalToString error");
}

std::string PairValue::toString() const {
    return "(" + internalToString() + ")";
}

std::vector<ValuePtr> PairValue::toVector() {//PairValue转vector
    std::vector<ValuePtr> result;
    ValuePtr temp = std::make_shared<PairValue>(*this);
    while (temp->isPair()) {
        result.push_back(std::dynamic_pointer_cast<PairValue>(temp)->car);
        temp = std::dynamic_pointer_cast<PairValue>(temp)->cdr;
    }
    if (temp && !temp->isNil()) {
        result.push_back(temp);
    }
    return result;
}

ValuePtr PairValue::getCar() const {
    return car;
}

ValuePtr PairValue::getCdr() const {
    return cdr;
}

std::shared_ptr<Value> PairValue::toQuote() {
    return std::make_shared<PairValue>(car->toQuote(), cdr->toQuote());
}

PairValue::PairValue(const std::vector<ValuePtr> &values) {
    ValuePtr result=std::make_shared<NilValue>();
    for(const auto & value : std::ranges::reverse_view(values)){
        result=std::make_shared<PairValue>(value,result);
    }
    auto pairResult=std::dynamic_pointer_cast<PairValue>(result);
    this->car=pairResult->car;
    this->cdr=pairResult->cdr;
}

std::string StringValue::toString() const {
    std::stringstream ss;
    ss << std::quoted(value);
    return ss.str();
}

std::string StringValue::internalToString() const {
    return value;
}

ValuePtr StringValue::toQuote() {
    return shared_from_this();
}


std::string SymbolValue::toString() const {
    return value;
}

std::shared_ptr<Value> Value::toQuote() {
    return std::make_shared<SymbolValue>(internalToString());
}

std::optional<std::string> Value::asSymbol() const {
    if (isSymbol()) {
        return std::static_pointer_cast<const SymbolValue>(shared_from_this())->toString();
    }
    return std::nullopt;
}

std::optional<double> Value::asNumber() const {
    if (isNum()) {
        return std::static_pointer_cast<const NumericValue>(shared_from_this())->getValue();
    }
    return std::nullopt;
}

std::optional<bool> Value::asBool() {
    if(isBool()){
        return std::dynamic_pointer_cast<BooleanValue>(shared_from_this())->getValue();
    }
    return true;
}

std::shared_ptr<Value> BuiltinProcValue::toQuote() {
    return std::make_shared<SymbolValue>(name);
}

std::string BuiltinProcValue::toString() const {
    return "#<procedure:"+name+">";//改进：返回过程的名字
}

double NumericValue::getValue() const {
    return value;
}

ValuePtr NumericValue::toQuote() {
    return shared_from_this();
}

std::string LambdaValue::toString() const {
    return "#<procedure>";
}

LambdaValue::LambdaValue(std::vector<std::string> params, std::vector<ValuePtr> body): params(std::move(params)), body(std::move(body)) {}

LambdaValue::LambdaValue(std::vector<std::string> params, std::vector<ValuePtr> body, std::shared_ptr<EvalEnv> env): params(std::move(params)), body(std::move(body)) {
    this->lambdaEnv = std::move(env);
}

ValuePtr LambdaValue::apply(const std::vector<ValuePtr> &args) {
    if (args.size() < params.size()) {//创建子表达式
        std::shared_ptr<LambdaValue> childLambda = std::make_shared<LambdaValue>(params, body, lambdaEnv->createChild());
        for(auto i=0;i<args.size();i++){//将参数绑定到子表达式的环境中
            childLambda->lambdaEnv->defineBinding(params[i],args[i]);
            childLambda->params.erase(childLambda->params.begin());//删除已经绑定的参数
        }
        return childLambda;
    }
    ValuePtr result;
    for (size_t i = 0; i < params.size(); i++) {
        lambdaEnv->defineBinding(params[i], args[i]);
    }if(body.size()==1){
        result=lambdaEnv->eval(body);
    }else {
        auto resultVector=std::vector<ValuePtr>();
        for(auto i=0;i<body.size();i++){
            resultVector.push_back(lambdaEnv->eval(body[i]));
        }
        result=lambdaEnv->eval(resultVector);
    }
    return result;
}

ValuePtr SymbolValue::toQuote() {
    return shared_from_this();
}