//
// Created by 王愉博 on 24-5-23.
//

#ifndef MINI_LISP_FORMS_H
#define MINI_LISP_FORMS_H

#include <unordered_map>
#include <vector>
class Value;
class EvalEnv;


using ValuePtr = std::shared_ptr<Value>;
using SpecialFormType = ValuePtr(const std::vector<ValuePtr> &, EvalEnv &);//定义一个函数指针类型
extern const std::unordered_map<std::string, SpecialFormType *> SPECIAL_FORMS;//特殊形式的map
ValuePtr defineForm(const std::vector<ValuePtr> &, EvalEnv &);
ValuePtr quoteForm(const std::vector<ValuePtr>& params, EvalEnv& env);//quote特殊形式的函数
ValuePtr ifForm(const std::vector<ValuePtr>& params, EvalEnv& env);//if特殊形式的函数
ValuePtr andForm(const std::vector<ValuePtr>& params, EvalEnv& env);//and特殊形式的函数
ValuePtr orForm(const std::vector<ValuePtr>& params, EvalEnv& env);//or特殊形式的函数
ValuePtr lambdaForm(const std::vector<ValuePtr>& params, EvalEnv& env);//lambda特殊形式的函数
#endif //MINI_LISP_FORMS_H
