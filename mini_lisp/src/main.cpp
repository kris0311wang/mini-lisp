#include <iostream>
#include <string>

#include "./tokenizer.h"
#include "parser.h"
#include "eval_env.h"
#include "rjsj_test.hpp"
#include"value.h"
#include"builtins.h"
#include"error.h"
#include"token.h"
#include<fstream>
#include<memory>
#include<stack>
#include"modes.h"

struct TestCtx {
    std::shared_ptr<EvalEnv> env = EvalEnv::createGlobal();

    std::string eval(const std::string &input) const {
        auto tokens = Tokenizer::tokenize(input);
        Parser parser(std::move(tokens));
        auto value = parser.parse();
        auto result = env->eval(value);
        return result->toString();
    }
};


int main(int argc, char **argv) {
    RJSJ_TEST(TestCtx, Lv2, Lv3, Lv4, Lv5, Lv6, Lv7, Lv7Lib, Sicp);
    if (argc == 1) {
        REPLmode();
    } else if (argc == 2) {
        FILEmode(std::string(argv[1]));
    } else {
        std::cerr << "Error: too many arguments" << std::endl;
        exit(1);
    }
}

