//
// Created by 王愉博 on 2024/5/15.
//

#ifndef MINI_LISP_BUILTINS_H
#define MINI_LISP_BUILTINS_H


#include<unordered_map>


extern std::unordered_map<std::string, std::shared_ptr<BuiltinProcValue>> builtin_funcs;

ValuePtr add(const std::vector<ValuePtr> &params);

ValuePtr print(const std::vector<ValuePtr> &params);

ValuePtr sub(const std::vector<ValuePtr> &params);

ValuePtr greaterThan(const std::vector<ValuePtr> &params);

ValuePtr lessThan(const std::vector<ValuePtr> &params);

ValuePtr length(const std::vector<ValuePtr> &params);

ValuePtr car(const std::vector<ValuePtr> &params);

ValuePtr cdr(const std::vector<ValuePtr> &params);

ValuePtr multiply(const std::vector<ValuePtr> &params);

#endif  // MINI_LISP_BUILTINS_H