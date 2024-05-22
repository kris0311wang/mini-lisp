//
// Created by 王愉博 on 24-5-23.
//

#ifndef MINI_LISP_EVAL_ENV_H
#define MINI_LISP_EVAL_ENV_H
#include "./value.h"
class EvalEnv {
public:
    ValuePtr eval(ValuePtr expr);

};
#endif //MINI_LISP_EVAL_ENV_H
