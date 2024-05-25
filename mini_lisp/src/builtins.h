//
// Created by 王愉博 on 2024/5/15.
//

#ifndef MINI_LISP_BUILTINS_H
#define MINI_LISP_BUILTINS_H


#include<unordered_map>


extern std::unordered_map<std::string, std::shared_ptr<BuiltinProcValue>> builtin_funcs;

ValuePtr add(const std::vector<ValuePtr> &params, EvalEnv &env);

ValuePtr print(const std::vector<ValuePtr> &params, EvalEnv &env);

ValuePtr sub(const std::vector<ValuePtr> &params, EvalEnv &env);

ValuePtr greaterThan(const std::vector<ValuePtr> &params, EvalEnv &env);

ValuePtr lessThan(const std::vector<ValuePtr> &params, EvalEnv &env);

ValuePtr length(const std::vector<ValuePtr> &params, EvalEnv &env);

ValuePtr car(const std::vector<ValuePtr> &params, EvalEnv &env);

ValuePtr cdr(const std::vector<ValuePtr> &params, EvalEnv &env);

ValuePtr multiply(const std::vector<ValuePtr> &params, EvalEnv &env);

ValuePtr apply(const std::vector<ValuePtr> &params, EvalEnv &env);
#endif  // MINI_LISP_BUILTINS_H