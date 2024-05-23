//
// Created by 王愉博 on 2024/5/15.
//
#include"value.h"
#include"builtins.h"
#include"error.h"

std::unordered_map<std::string, std::shared_ptr<BuiltinProcValue>> builtin_funcs{};  // 内建函数的map
ValuePtr add(const std::vector<ValuePtr> &params) {//add函数的实现:输入是一个ValuePtr的vector，输出是一个ValuePtr
    double result = 0;
    for (const auto &i: params) {
        if (i->isNum()) {
            result += *i->asNumber();
        } else {
            throw LispError("add: arguments must be numbers.");
        }
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr print(const std::vector<ValuePtr> &params) {//print函数的实现:输入是一个ValuePtr的vector，输出空表
    for (const auto &i: params) {
        std::cout << i->toString() << " ";
    }
    std::cout << std::endl;
    return std::make_shared<NilValue>();
}

void init_builtins() {//初始化内建函数
    builtin_funcs["+"] = std::make_shared<BuiltinProcValue>("+", add);
    builtin_funcs["print"] = std::make_shared<BuiltinProcValue>("print", print);
}