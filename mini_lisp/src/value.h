//
// Created by 王愉博 on 24-5-23.
//

#ifndef MINI_LISP_VALUE_H
#define MINI_LISP_VALUE_H

#include<iostream>
#include<memory>
#include<string>
#include<utility>
#include<vector>
#include<optional>

class EvalEnv;

class Value : public std::enable_shared_from_this<Value> {
public:
    virtual bool operator==(const Value &rhs) const = 0;

    virtual std::string toString() const = 0;

    virtual ~Value() = default;

    bool isNil() const;

    bool isSymbol() const;

    virtual std::optional<std::string> asSymbol() const;

    bool isNum() const;

    bool isBool() const;

    std::optional<bool> asBool();

    bool isString() const;

    bool isPair() const;

    bool isList() const;

    bool isSelfEvaluating() const;

    virtual std::vector<std::shared_ptr<Value>> toVector();

    virtual std::shared_ptr<Value> toQuote();

    std::optional<double> asNumber() const;

    bool isBuiltin() const;

    virtual bool isInt() const;

    bool isLambda() const;

    bool isAtom() const;

};

using ValuePtr = std::shared_ptr<Value>;

class BooleanValue : public Value {
    bool value;
public:
    bool operator==(const Value &rhs) const override;

    bool getValue() const;

    explicit BooleanValue(const bool &value) : value(value) {}

    std::string toString() const override;

    ValuePtr toQuote() override;
};

class NumericValue : public Value {
    double value;
public:
    bool operator==(const Value &rhs) const override;

    explicit NumericValue(const double &value) : value(value) {}

    bool isInt() const override;

    std::string toString() const override;

    double getValue() const;

    ValuePtr toQuote() override;

    void setValue(const double &value);
};

class NilValue : public Value {
public:
    bool operator==(const Value &rhs) const override;

    std::string toString() const override;

    std::shared_ptr<Value> toQuote() override;

    std::vector<std::shared_ptr<Value>> toVector() override;
};

class PairValue : public Value {
    ValuePtr car;
    ValuePtr cdr;
public:
    bool operator==(const Value &rhs) const override;

    explicit PairValue(const ValuePtr &car, const ValuePtr &cdr) : car(car->shared_from_this()),
                                                                   cdr(cdr->shared_from_this()) {}

    explicit PairValue(const std::vector<ValuePtr> &values);


    std::string toString() const override;

    ValuePtr getCar() const;

    ValuePtr getCdr() const;

    void append(const ValuePtr &valueptr);

    std::vector<std::shared_ptr<Value>> toVector() override;

    std::shared_ptr<Value> toQuote() override;
};

class SymbolValue : public Value {
    std::string value;
public:
    bool operator==(const Value &rhs) const override;

    explicit SymbolValue(std::string value) : value(std::move(value)) {}

    std::string toString() const override;

    ValuePtr toQuote() override;
};

class StringValue : public Value {
    std::string value;
public:
    std::string getValue();

    bool operator==(const Value &rhs) const override;

    explicit StringValue(std::string value) : value(std::move(value)) {}

    std::string toString() const override;

    ValuePtr toQuote() override;
};

using BuiltinFuncType = ValuePtr(const std::vector<ValuePtr> &, EvalEnv &env);//内建函数的函数指针类型
class BuiltinProcValue : public Value {
public:
    bool operator==(const Value &rhs) const override;

    std::string name;
    BuiltinFuncType *func;//函数指针
    BuiltinProcValue(std::string name, BuiltinFuncType func) : name(std::move(name)), func(func) {}//构造函数
    std::shared_ptr<Value> toQuote() override;

    std::string toString() const override;
};

class LambdaValue : public Value {
private:
    std::vector<std::string> params;
    std::vector<ValuePtr> body;
    std::shared_ptr<EvalEnv> lambdaEnv;
    // [...]
public:
    bool operator==(const Value &rhs) const override;

    LambdaValue(std::vector<std::string> params, std::vector<ValuePtr> body);

    LambdaValue(std::vector<std::string> params, std::vector<ValuePtr> body, std::shared_ptr<EvalEnv> env);

    std::string toString() const override; // 如前所述，返回 #<procedure> 即可
    ValuePtr apply(const std::vector<ValuePtr> &args);
};

#endif //MINI_LISP_VALUE_H
