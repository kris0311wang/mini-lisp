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
    return shared_from_this();//对于布尔值，quote返回自身
}

bool BooleanValue::operator==(const Value &rhs) const {
    if(!rhs.isBool()){
        return false;
    }
    return value==std::dynamic_pointer_cast<const BooleanValue>(rhs.shared_from_this())->getValue();
}

bool NumericValue::operator==(const Value &rhs) const {
    if(!rhs.isNum()){
        return false;
    }
    return value==std::dynamic_pointer_cast<const NumericValue>(rhs.shared_from_this())->getValue();
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

std::shared_ptr<Value> NilValue::toQuote() {
    return shared_from_this();//对于空表，quote返回空表
}

std::vector<std::shared_ptr<Value>> NilValue::toVector() {
    return {};
}

bool NilValue::operator==(const Value &rhs) const {
    return rhs.isNil();
}

bool SymbolValue::operator== (const Value &rhs) const {
    if(!rhs.isSymbol()){
        return false;
    }
    return value==std::dynamic_pointer_cast<const SymbolValue>(rhs.shared_from_this())->value;
}

bool StringValue::operator==(const Value &rhs) const {
    if(!rhs.isString()){
        return false;
    }
    return value==std::dynamic_pointer_cast<const StringValue>(rhs.shared_from_this())->value;
}

bool BuiltinProcValue::operator==(const Value &rhs) const {
    if(!rhs.isBuiltin()){
        return false;
    }
    throw LispError("BuiltinProcValue operator== undefined!");
}

bool LambdaValue::operator==(const Value &rhs) const {
    throw LispError("LambdaValue operator== undefined!");
}

bool PairValue::operator==(const Value &rhs) const {
    if(!rhs.isPair()){
        return false;
    }
    auto pair=std::dynamic_pointer_cast<const PairValue>(rhs.shared_from_this());
    return *car==*pair->car&&*cdr==*pair->cdr;
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
    return typeid(*this) == typeid(BuiltinProcValue);
}

bool Value::isLambda() const {
    return typeid(*this) == typeid(LambdaValue);
}

bool Value::isInt() const{
    return false;
}


bool Value::isSelfEvaluating() const {
    return isNum() || isBool() || isString() ;
}

std::vector<std::shared_ptr<Value>> Value::toVector() {
    return {shared_from_this()};
}

//std::string PairValue::internalToString() const {
//    if (cdr->isNil()) {
//        return car->internalToString();
//    } else if (!cdr->isPair() && cdr) {
//        return car->internalToString() + " . " + cdr->internalToString();
//    } else if (cdr) {
//        return car->internalToString() + " " + cdr->internalToString();
//    }
//    throw LispError("PairValue internalToString error");
//}

std::string PairValue::toString() const {
    if(isList()){
        std::stringstream ss;
        ss<<"(";
        auto temp=std::make_shared<PairValue>(*this);
        while(true){
            ss<<temp->car->toString();
            if(temp->cdr->isNil()){
                break;
            }
            temp=std::dynamic_pointer_cast<PairValue>(temp->cdr);
            ss<<" ";
        }
        ss<<")";
        return ss.str();
    }else {
        std::string result = "(" + car->toString() + " . " + cdr->toString() + ")";
        return result;
    }
}

std::vector<std::shared_ptr<Value>> PairValue::toVector() {//PairValue转vector
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

void PairValue::append(const ValuePtr &valueptr) {
    auto temp=std::make_shared<PairValue>(*this);
    while(temp->cdr->isPair()){
        temp=std::dynamic_pointer_cast<PairValue>(temp->cdr);
    }
    if(temp->cdr->isNil()){//当前对象是list
        temp->cdr=valueptr;
    }
    else{
        throw LispError("PairValue::append error: params should be lists.");
    }
}
std::shared_ptr<Value> PairValue::toQuote() {
    return std::make_shared<PairValue>(car->toQuote(), cdr->toQuote());//对于对子，quote递归调用toquote最后返回一个对子
}

PairValue::PairValue(const std::vector<ValuePtr> &values) {
    ValuePtr result = std::make_shared<NilValue>();
    for (const auto &value: std::ranges::reverse_view(values)) {
        result = std::make_shared<PairValue>(value, result);
    }
    auto pairResult = std::dynamic_pointer_cast<PairValue>(result);
    this->car = pairResult->car;
    this->cdr = pairResult->cdr;
}

std::string StringValue::toString() const {
    std::stringstream ss;
    ss << std::quoted(value);
    return ss.str();
}


ValuePtr StringValue::toQuote() {
    return shared_from_this();//对于字符串，quote返回自身
}


std::string SymbolValue::toString() const {
    return value;
}


std::shared_ptr<Value> Value::toQuote() {
    return std::make_shared<SymbolValue>(toString());//对于其他类型，quote返回自身的字符串表示
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
    if (isBool()) {
        return std::dynamic_pointer_cast<BooleanValue>(shared_from_this())->getValue();
    }
    return true;
}

bool Value::isList() const {//如果最后的cdr是NilValue就是list
    if(isNil()){
        return true;
    }
    if(isPair()){
        auto pairvalue=std::static_pointer_cast<const PairValue>(std::move(shared_from_this()));
        return pairvalue->getCdr()->isList();
    }
    return false;
}

std::shared_ptr<Value> BuiltinProcValue::toQuote() {
    return std::make_shared<SymbolValue>(name);//对于内置过程，quote返回过程的名字
}

std::string BuiltinProcValue::toString() const {
    return "#<procedure:" + name + ">";//改进：返回过程的名字
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

LambdaValue::LambdaValue(std::vector<std::string> params, std::vector<ValuePtr> body) : params(std::move(params)),
                                                                                        body(std::move(body)) {}

LambdaValue::LambdaValue(std::vector<std::string> params, std::vector<ValuePtr> body, std::shared_ptr<EvalEnv> env)
        : params(std::move(params)), body(std::move(body)) {
    this->lambdaEnv = std::move(env);
}

ValuePtr LambdaValue::apply(const std::vector<ValuePtr> &args) {
    if (args.size() < params.size()) {//传入参数不够则创建子表达式
        std::shared_ptr<LambdaValue> childLambda = std::make_shared<LambdaValue>(params, body,
                                                                                 lambdaEnv->createChild());
        for (auto i = 0; i < args.size(); i++) {//将参数绑定到子表达式的环境中
            childLambda->lambdaEnv->defineBinding(params[i], args[i]);
            childLambda->params.erase(childLambda->params.begin());//删除已经绑定的参数
        }
        return childLambda;
    }
    ValuePtr result;
    for (size_t i = 0; i < params.size(); i++) {
        lambdaEnv->defineBinding(params[i], args[i]);
    }
    result= lambdaEnv->eval(body);
    return result;
}

ValuePtr SymbolValue::toQuote() {
    return shared_from_this();//对于符号，quote返回自身
}

std::string StringValue::getValue(){
    return value;
}
