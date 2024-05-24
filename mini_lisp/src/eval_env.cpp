//
// Created by 王愉博 on 24-5-23.
//
#include "eval_env.h"
#include"error.h"
#include"forms.h"
#include<algorithm>
#include<iterator>
#include <utility>
#include "./value.h"
#include <unordered_map>
#include"builtins.h"

ValuePtr EvalEnv::eval(ValuePtr expr) {
    if (expr->isSelfEvaluating()) {
        return expr;
    } else if (expr->isNil()) {
        throw LispError("Evaluating nil is prohibited.");
    } else if (expr->isSymbol()) {
        return lookupBinding(*expr->asSymbol());
    } else if (expr->isPair()) {
        auto pairExpr = std::dynamic_pointer_cast<PairValue>(expr);
        if (pairExpr->getCar()->isSymbol()) {//如果第一个元素是符号
            auto name = pairExpr->getCar()->asSymbol();
            if (SPECIAL_FORMS.find(*name) != SPECIAL_FORMS.end()) {//如果是特殊形式,调用特殊形式
                return SPECIAL_FORMS.at(*name)(pairExpr->getCdr()->toVector(), *this);
            } else {//如果不是特殊形式,调用apply函数
                ValuePtr proc = lookupBinding(*name);
                std::vector<ValuePtr> args = evalList(pairExpr->getCdr());
                return apply(proc, args);
            }
        }
    } else {
        throw LispError("Unimplemented");
    }
}

EvalEnv::EvalEnv() : parent(nullptr) {//初始化符号表,将内置函数添加到符号表中,并将parent指针初始化为nullptr
    for (auto &i: builtin_funcs) {
        defineBinding(i.first, i.second);
    }
}

void EvalEnv::defineBinding(const std::string &name, const ValuePtr &value) {//将name和value添加到符号表中
    symbolTable[name] = value;
}

ValuePtr EvalEnv::lookupBinding(const std::string &name) const {//查找name在符号表中的值,如果没有找到,则递归查找parent
    auto it = symbolTable.find(name);
    if (it == symbolTable.end()) {
        if (parent) {
            return parent->lookupBinding(name);
        } else {
            throw LispError("Unbound symbol: " + name);
        }
    }
    return it->second;
}

std::vector<ValuePtr> EvalEnv::evalList(const ValuePtr &expr) {
    if (expr->isNil()) {
        return {};
    }
    std::vector<ValuePtr> result;
    std::ranges::transform(expr->toVector(),
                           std::back_inserter(result),
                           [this](ValuePtr v) { return this->eval(std::move(v)); });
    return result;
}

ValuePtr EvalEnv::apply(const ValuePtr &proc, const std::vector<ValuePtr> &args) {
    if (typeid(*proc) == typeid(BuiltinProcValue)) {
        // 调用内置过程
        return std::static_pointer_cast<BuiltinProcValue>(proc)->func(args);
    } else {
        throw LispError("Unimplemented");
    }
}

std::shared_ptr<EvalEnv> EvalEnv::createChild() {
    auto child = std::make_shared<EvalEnv>();
    child->parent = shared_from_this();
    return child;
}

std::shared_ptr<EvalEnv> EvalEnv::createGlobal() {
    auto global=EvalEnv();
    return std::make_shared<EvalEnv>(global);
}


