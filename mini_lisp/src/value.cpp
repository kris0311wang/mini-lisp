//
// Created by 王愉博 on 24-5-23.
//
#include"value.h"
#include <sstream>
#include <iomanip>
#include <vector>
#include <optional>

std::string BooleanValue::toString() const {
    return value ? "#t" : "#f";
}

bool BooleanValue::getValue() const {
    return value;
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

std::string Value::internalToString() const {
    return toString();
}

bool Value::isSelfEvaluating() const {
    return isNum() || isBool() || isString();
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
    return "#<procedure>";
}

double NumericValue::getValue() const {
    return value;
}

std::string LambdaValue::toString() const {
    return "#<procedure>";
}

LambdaValue::LambdaValue(std::vector<std::string> params, std::vector<ValuePtr> body) : params(std::move(params)), body(std::move(body)) {}
