//
// Created by 王愉博 on 2024/5/15.
//

#ifndef MINI_LISP_BUILTINS_H
#define MINI_LISP_BUILTINS_H

#include "value.h"
#include<unordered_map>

extern std::unordered_map<std::string, std::shared_ptr<BuiltinProcValue>> builtin_funcs;

ValuePtr add(const std::vector<ValuePtr> &params);

ValuePtr print(const std::vector<ValuePtr> &params);

void init_builtins();

#endif  // MINI_LISP_BUILTINS_H