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

class EvalEnv {
    std::unordered_map<std::string, ValuePtr> symbolTable;//unimplemented
    std::shared_ptr<EvalEnv> parent;
public:
    EvalEnv();

    ValuePtr eval(ValuePtr expr);

    void defineBinding(const std::string &name, const ValuePtr &value);

    ValuePtr lookupBinding(const std::string &name) const;

    static ValuePtr apply(const ValuePtr &proc, const std::vector<ValuePtr> &args);

    std::vector<ValuePtr> evalList(const ValuePtr &expr);
};

#endif //MINI_LISP_EVAL_ENV_H
