//
// Created by 王愉博 on 24-5-23.
//

#ifndef MINI_LISP_EVAL_ENV_H
#define MINI_LISP_EVAL_ENV_H

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

class Value;

using ValuePtr = std::shared_ptr<Value>;

class EvalEnv : public std::enable_shared_from_this<EvalEnv> {
    std::unordered_map<std::string, ValuePtr> symbolTable;//符号表
    std::shared_ptr<EvalEnv> parent = nullptr;//父节点

    EvalEnv(bool root);//只有根节点需要添加内置符号表，同时设置成private用只能指针管理
public:

    ValuePtr eval(ValuePtr expr);

    ValuePtr eval(const std::vector<ValuePtr> &expr);

    void defineBinding(const std::string &name, const ValuePtr &value);

    ValuePtr lookupBinding(const std::string &name) const;

    static ValuePtr apply(const ValuePtr &proc, const std::vector<ValuePtr> &args, EvalEnv &env);

    std::vector<ValuePtr> evalList(const ValuePtr &expr);

    std::shared_ptr<EvalEnv> createChild();

    static std::shared_ptr<EvalEnv> createGlobal();

    std::vector<ValuePtr> evalList(const std::vector<ValuePtr> &expr);
};

#endif //MINI_LISP_EVAL_ENV_H
