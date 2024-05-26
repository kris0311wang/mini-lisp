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
    if (expr->isSelfEvaluating()) {//递归结束条件：得到一个自求值类型的值
        return expr;
    } else if (expr->isNil()) {//计算空表抛出异常
        throw LispError("Evaluating nil is prohibited.");
    } else if (expr->isSymbol()) {//如果是符号,则在符号表中查找，直到得到一个自求值类型的值
        return eval(lookupBinding(*expr->asSymbol()));
    } else if (expr->isPair()) {//如果是对子类型，考虑到特殊形式，分类讨论
        auto pairExpr = std::dynamic_pointer_cast<PairValue>(expr);
        if (pairExpr->getCar()->isSymbol()) {//如果第一个元素是符号
            auto name = pairExpr->getCar()->asSymbol();
            if (SPECIAL_FORMS.find(*name) != SPECIAL_FORMS.end()) {//如果是特殊形式,调用特殊形式
                return SPECIAL_FORMS.at(*name)(pairExpr->getCdr()->toVector(), *this);
            } else {//如果不是特殊形式,则是过程，解析出符号,然后解析参数表，最后调用apply函数
                ValuePtr proc = eval(pairExpr->getCar());
                std::vector<ValuePtr> args = evalList(pairExpr->getCdr());//递归eval得到最简参数表
                return apply(proc, args, *this);
            }
        }else if(pairExpr->getCar()->isBuiltin()){//如果第一个元素是内置函数,调用
            return apply(pairExpr->getCar(), evalList(pairExpr->getCdr()), *this);
        }
        else if(pairExpr->getCar()->isPair()){//如果第一个元素是表达式，计算
            return eval(evalList(expr));
        }
        throw LispError("Unimplemented123");
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
    if (typeid(*proc) == typeid(BuiltinProcValue)) {
        // 调用内置过程
        return std::static_pointer_cast<BuiltinProcValue>(proc)->func(args, env);
    } else if(typeid(*proc)==typeid(LambdaValue)){
        return std::static_pointer_cast<LambdaValue>(proc)->apply(args);
    }
    else {
        throw LispError("apply not a procedure");
    }
}

ValuePtr EvalEnv::eval(const std::vector<ValuePtr> &expr) {//移植valuePtr的计算，但是在内部计算
    if(expr.empty()){
        return std::make_shared<NilValue>();
    }else if(expr.size()==1 && expr[0]->isSelfEvaluating()){
        return expr[0];
    }
    else if (expr[0]->isSymbol()) {//如果第一个元素是符号
        auto name = expr[0]->asSymbol();
        if (SPECIAL_FORMS.find(*name) != SPECIAL_FORMS.end()) {//如果是特殊形式,调用特殊形式
            return SPECIAL_FORMS.at(*name)(std::vector<ValuePtr>(expr.begin()+1,expr.end()), *this);
        } else {//如果不是特殊形式,调用apply函数
            ValuePtr proc = lookupBinding(*name);
            std::vector<ValuePtr> args = evalList(std::vector<ValuePtr>(expr.begin()+1,expr.end()));
            return apply(proc, args, *this);
        }
    }else if(expr[0]->isBuiltin()){//如果第一个元素是内置函数,调用
        return apply(expr[0], evalList(std::vector<ValuePtr>(expr.begin() + 1, expr.end())), *this);
    }
    else if(expr[0]->isPair()&&expr.size()==1){//如果第一个元素是表达式，计算
        return eval(expr[0]);
    }else if(expr[0]->isNil()){//如果第一个元素是空表,则弹出
        return eval(std::vector<ValuePtr>(expr.begin()+1,expr.end()));
    }
    throw LispError("evalvector Unimplemented");
}

std::shared_ptr<EvalEnv> EvalEnv::createChild() {
    auto child =EvalEnv();
    child.parent = shared_from_this();
    return std::make_shared<EvalEnv>(child);
}

std::shared_ptr<EvalEnv> EvalEnv::createGlobal() {
    auto global=EvalEnv();
    return std::make_shared<EvalEnv>(global);
}


