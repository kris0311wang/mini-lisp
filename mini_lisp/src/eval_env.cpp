//
// Created by 王愉博 on 24-5-23.
//
#include "eval_env.h"
#include"error.h"
ValuePtr EvalEnv::eval(ValuePtr expr) {
    if (expr->isSelfEvaluating()) {
        return expr;
    } else if (expr->isNil()) {
        throw LispError("Evaluating nil is prohibited.");
    } else {
        throw LispError("Unimplemented");
    }return nullptr;
}

