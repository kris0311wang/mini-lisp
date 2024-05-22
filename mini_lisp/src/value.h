//
// Created by 王愉博 on 24-5-23.
//

#ifndef MINI_LISP_VALUE_H
#define MINI_LISP_VALUE_H
#include<iostream>
#include<memory>
#include<string>
class Value:public std::enable_shared_from_this<Value>{
public:
    [[nodiscard]] virtual std::string internalToString() const;
    [[nodiscard]] virtual std::string toString() const = 0;
    virtual ~Value() = default;
    [[nodiscard]] bool isNil() const;
    [[nodiscard]] bool isSymbol() const;
    [[nodiscard]] bool isNum() const;
    [[nodiscard]] bool isBool() const;
    [[nodiscard]] bool isString() const;
    [[nodiscard]] bool isPair() const;
};
using ValuePtr = std::shared_ptr<Value>;
class BooleanValue:public Value{
    bool value;
public:
    explicit BooleanValue(const bool& value):value(value){}
    [[nodiscard]] std::string toString() const override ;
};
class NumericValue:public Value{
    double value;
public:
    explicit NumericValue(const double& value):value(value){}
    [[nodiscard]] bool isInt()const;
    [[nodiscard]] std::string toString() const override ;
};
class NilValue:public Value{
public:
    [[nodiscard]] std::string internalToString () const override;
    [[nodiscard]] std::string toString() const override ;
};
class PairValue:public Value{
    ValuePtr car;
    ValuePtr cdr;
public:
    explicit PairValue(const ValuePtr& car,const ValuePtr&  cdr): car(car->shared_from_this()), cdr(cdr->shared_from_this()){}
    [[nodiscard]] std::string internalToString() const override;
    [[nodiscard]] std::string toString() const override;
};
class SymbolValue:public Value {
    std::string value;
public:
    explicit SymbolValue(std::string value):value(std::move(value)){}
    [[nodiscard]] std::string toString() const override;
};
class StringValue:public Value{
    std::string value;
public:
    explicit StringValue(std::string  value):value(std::move(value)){}
    [[nodiscard]] std::string internalToString() const override;
    [[nodiscard]] std::string toString() const override;
};
#endif //MINI_LISP_VALUE_H
