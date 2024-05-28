#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
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
    if (expr->isSelfEvaluating()||expr->isBuiltin()||expr->isLambda()) {//递归结束条件：得到一个自求值类型的值,或者得到过程量
        return expr;
    } else if (expr->isNil()) {//计算空表抛出异常
        throw LispError("Evaluating nil is prohibited.");
    } else if (expr->isSymbol()) {//如果是符号,则在符号表中查找
        return lookupBinding(*expr->asSymbol());
    }  else if (expr->isPair()) {//如果是对子类型，第一个元素一共有两种情况：特殊形式或者过程。由于特殊形式不能被命名，所以先考虑特殊形式，剩下的就是过程
        auto pairExpr = std::dynamic_pointer_cast<PairValue>(expr);
        if (pairExpr->getCar()->isSymbol()) {//如果第一个元素是符号
            auto name = pairExpr->getCar()->asSymbol();
            if (SPECIAL_FORMS.find(*name) != SPECIAL_FORMS.end()) {//如果是特殊形式,调用特殊形式
                return SPECIAL_FORMS.at(*name)(pairExpr->getCdr()->toVector(), *this);
            }
        } //如果不是特殊形式,则是过程，解析出过程,然后解析参数表，最后调用apply函数
        ValuePtr proc = eval(pairExpr->getCar());
        std::vector<ValuePtr> args = evalList(pairExpr->getCdr());//递归eval得到最简参数表
        return apply(proc, args, *this);
    }
    else {
        throw LispError("Unimplemented");
    }
}

EvalEnv::EvalEnv() : parent(nullptr) {//初始化符号表,将内置函数添加到符号表中,并将parent指针初始化为nullptr
    for (auto &i: builtin_funcs) {
        defineBinding(i.first, i.second);
    }
    defineBinding ("else",std::make_shared<SymbolValue>("else"));//else仅在cond中使用，特殊处理
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

std::vector<ValuePtr> EvalEnv::evalList(const ValuePtr &expr) {//对expr中的每个元素调用eval函数,并转化为vector输出
    if (expr->isNil()) {
        return {};
    }
    std::vector<ValuePtr> result;
    std::ranges::transform(expr->toVector(),
                           std::back_inserter(result),
                           [this](ValuePtr v) { return this->eval(std::move(v)); });
    return result;
}

std::vector<ValuePtr> EvalEnv::evalList(const std::vector<ValuePtr> &expr) {//对expr中的每个元素调用eval函数
    if (expr.empty()) {
        return {};
    }
    std::vector<ValuePtr> result;
    std::ranges::transform(expr,
                           std::back_inserter(result),
                           [this](ValuePtr v) { return this->eval(std::move(v)); });
    return result;
}

ValuePtr EvalEnv::apply(const ValuePtr &proc, const std::vector<ValuePtr> &args, EvalEnv &env) {
    if (proc->isBuiltin()) {
        // 调用内置过程
        return std::static_pointer_cast<BuiltinProcValue>(proc)->func(args, env);
    } else if (proc->isLambda()) {
        return std::static_pointer_cast<LambdaValue>(proc)->apply(args);
    } else {
        throw LispError("apply not a procedure");
    }
}

ValuePtr EvalEnv::eval(const std::vector<ValuePtr> &expr) {//移植valuePtr的计算，仅内部计算使用
    if (expr.empty()) {//如果表达式为空,返回空表，递归结束条件之一，其余情况通过递归计算第一个元素得到
        return std::make_shared<NilValue>();
    } else if (expr[0]->isNil()) {//如果第一个元素是空表,则弹出
        return eval(std::vector<ValuePtr>(expr.begin() + 1, expr.end()));
    }else if (expr.size() == 1 ) {//移植
        return eval(expr[0]);
    }//接下来都是多于1个元素的情况。递归计算第一个元素
    else if(expr[0]->isSelfEvaluating()) {//如果第一个元素是自求值类型,则返回下一个表达式的结果
        return eval(std::vector<ValuePtr>(expr.begin() + 1, expr.end()));
    }
    else if (expr[0]->isSymbol()) {//如果第一个元素是符号
        auto name = expr[0]->asSymbol();
        if (SPECIAL_FORMS.find(*name) != SPECIAL_FORMS.end()) {//如果是特殊形式,调用特殊形式
            return SPECIAL_FORMS.at(*name)(std::vector<ValuePtr>(expr.begin() + 1, expr.end()), *this);
        } else {//如果不是特殊形式,那么第一个元素是过程，调用apply函数
            ValuePtr proc = eval(expr[0]);
            std::vector<ValuePtr> args = evalList(std::vector<ValuePtr>(expr.begin() + 1, expr.end()));
            return apply(proc, args, *this);
        }
    } else if (expr[0]->isBuiltin()||expr[0]->isLambda()) {//如果第一个元素是内置函数,调用
        return apply(expr[0], evalList(std::vector<ValuePtr>(expr.begin() + 1, expr.end())), *this);
    } else if (expr[0]->isPair() ) {//如果第一个元素是表达式，计算之后pop掉
        eval(expr[0]);
        return eval(std::vector<ValuePtr>(expr.begin() + 1, expr.end()));
    }
    throw LispError("evalVector Unimplemented");
}

std::shared_ptr<EvalEnv> EvalEnv::createChild() {
    auto child = EvalEnv();
    child.parent = shared_from_this();
    return std::make_shared<EvalEnv>(child);
}

std::shared_ptr<EvalEnv> EvalEnv::createGlobal() {
    auto global = EvalEnv();
    return std::make_shared<EvalEnv>(global);
}



#pragma clang diagnostic pop