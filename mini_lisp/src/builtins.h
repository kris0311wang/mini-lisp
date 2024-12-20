//
// Created by 王愉博 on 2024/5/15.
//

#ifndef MINI_LISP_BUILTINS_H
#define MINI_LISP_BUILTINS_H


#include<unordered_map>


extern std::unordered_map<std::string, std::shared_ptr<BuiltinProcValue>> builtin_funcs;

void checkExactSize(const std::vector<ValuePtr> &params, int size, const std::string &name);

void checkMinSize(const std::vector<ValuePtr> &params, int size, const std::string &name);

ValuePtr add(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr print(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr sub(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr greaterThan(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr lessThan(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr length(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr car(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr cdr(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr multiply(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr apply(const std::vector<ValuePtr> &params, EvalEnv &env);

ValuePtr display(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr displayln(const std::vector<ValuePtr> &params, EvalEnv &env);

ValuePtr error(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr eval(const std::vector<ValuePtr> &params, EvalEnv &env);

ValuePtr cons(const std::vector<ValuePtr> &params, [[maybe_unused]] [[maybe_unused]] EvalEnv &env);

ValuePtr builtinExit(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr newline(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr atomCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr booleanCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] [[maybe_unused]] EvalEnv &env);

ValuePtr intergerCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr listCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr numberCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr nullCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr pairCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr procedureCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr stringCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr symbolCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr append(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr list(const std::vector<ValuePtr> &params, [[maybe_unused]] [[maybe_unused]] EvalEnv &env);

ValuePtr map(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr reduce(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr divide(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr expt(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr quotient(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr ValueRemainder(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr modulo(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr equalCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr eqCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr notCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr equalSignCheck(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr greaterThanOrEqual(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr lessThanOrEqual(const std::vector<ValuePtr> &params, EvalEnv &env);

ValuePtr evenCheck(const std::vector<ValuePtr> &params, EvalEnv &env);

ValuePtr oddCheck(const std::vector<ValuePtr> &params, EvalEnv &env);

ValuePtr zeroCheck(const std::vector<ValuePtr> &params, EvalEnv &env);

ValuePtr filter(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

ValuePtr absoluteValue(const std::vector<ValuePtr> &params, [[maybe_unused]] EvalEnv &env);

#endif  // MINI_LISP_BUILTINS_H