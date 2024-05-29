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

ValuePtr excecuteLine(const std::string &line, std::shared_ptr<EvalEnv> &env) {//执行一个代码语句(单一对象或者括号匹配的语句)
    auto tokens = Tokenizer::tokenize(line);
    Parser parser(std::move(tokens));
    auto value = parser.parse();
    return env->eval(value);
}

bool isBalanced(const std::string &line) {//判断括号是否匹配
    std::stack<char> stack;
    for (char ch : line) {
        if (ch == '(') {
            stack.push(ch);
        } else if (ch == ')') {
            if (stack.empty()) {
                return false;
            }
            stack.pop();
        }
    }
    return stack.empty();
}



void REPLmode() {
    auto env = EvalEnv::createGlobal();
    while (true) {
        try {
            std::cout << ">>> ";
            std::string line;//代码语句（单一对象或者括号匹配的语句）
            std::string input;//输入的一行
            while (true) {//逐行读取输入，直到括号匹配，支持换行
                std::getline(std::cin, input);
                if (std::cin.eof()) {
                    exit(0);
                }
                line += input + " ";
                if (isBalanced(line)) {//括号匹配，退出循环
                    break;
                }
            }
            auto result=excecuteLine(line, env);
            std::cout << result->toString() << std::endl;

        } catch (std::runtime_error &e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

void FILEmode(std::string filename){
    std::cout<<"file mode: running "+filename<<std::endl;
    auto env = EvalEnv::createGlobal();
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: cannot open file " << filename << std::endl;
        exit(1);
    }
    std::string input;
    std::string line;
    while(true){
        try {
            if (file.eof()) {//运行完毕，退出码0正常退出
                exit(0);
            }
            std::getline(file, input);

        }
        catch (std::runtime_error &e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }

    }
}

int main(int argc, char **argv){
    if (argc==1) {
        REPLmode();
    }else if(argc==2){
        FILEmode(std::string(argv[1]));
    }
}

