//
// Created by 王愉博 on 24-5-23.
//
#include"value.h"
std::string BooleanValue::toString() const {
    return value ? "#t" : "#f";
}
std::string NumericValue::toString() const {
    return isInt() ? std::to_string(static_cast<int>(value)):std::to_string(value);
}
bool NumericValue::isInt() const{
    return value == static_cast<int>(value);
}
std::string NilValue::toString() const {
    return "()";
}

std::string NilValue::internalToString() const {
    return "";
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

std::string Value::internalToString() const{
    return toString();
}

std::string PairValue::internalToString() const {
    if(cdr->isNil()){
        return car->internalToString();
    }else if(!cdr->isPair()){
        return car->internalToString() + " . " + cdr->internalToString();
    }else{
        return car->internalToString() + " " + cdr->internalToString();
    }
}

std::string PairValue::toString() const {
    return "(" + internalToString() + ")";
}

std::string StringValue::internalToString() const {
    return value;
}
std::string StringValue::toString() const {
    return "\"" + value + "\"";
}

std::string SymbolValue::toString() const {
    return value;
}
