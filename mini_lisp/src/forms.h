//
// Created by 王愉博 on 24-5-23.
//

#ifndef MINI_LISP_FORMS_H
#define MINI_LISP_FORMS_H

#include"value.h"
#include"eval_env.h"

using SpecialFormType = ValuePtr(const std::vector<ValuePtr> &, EvalEnv &);//定义一个函数指针类型
extern const std::unordered_map<std::string, SpecialFormType *> SPECIAL_FORMS;//特殊形式的map
ValuePtr defineForm(const std::vector<ValuePtr> &, EvalEnv &);
ValuePtr quoteForm(const std::vector<ValuePtr>& params, EvalEnv& env);//quote特殊形式的函数

#endif //MINI_LISP_FORMS_H
