//
// Created by 王愉博 on 24-5-29.
//

#include "modes.h"
#include "tokenizer.h"
#include "parser.h"
#include "eval_env.h"
#include "value.h"
#include <iostream>
#include <fstream>
#include <memory>
#include "token.h"

ValuePtr excecuteLine(const std::string &line, std::shared_ptr<EvalEnv> &env) {//执行一个代码语句(单一对象或者括号匹配的语句)
    auto tokens = Tokenizer::tokenize(line);
    Parser parser(std::move(tokens));
    auto value = parser.parse();
    return env->eval(value);
}

bool containContent(std::string line){//用于REPL换行时判断是否有内容
    for(auto ch:line){
        if(ch!=' '&&ch!='\n')
            return true;
    }
    return false;
}

std::string parseCode(std::istream& is,bool REPL) {//解析一行代码语句
    std::string line;
    char ch;
    int balance = 0;

    while (is.get(ch)) {
        line += ch;
        if (ch == '(') {
            ++balance;
        } else if (ch == ')') {
            --balance;
            if (balance == 0) {
                break;
            }
        }else if(ch=='\n'){
            if(REPL){
                if(containContent(line))//REPL模式下换行时判断是否有内容
                    break;
            }
        }
    }
    if(is.eof()){//运行完毕，正常退出进程，代码为0
        exit(0);
    }

    return line;
}


void REPLmode() {
    auto env = EvalEnv::createGlobal();
    while (true) {
        try {
            std::cout << ">>> ";
            std::string line=parseCode(std::cin,true);
            auto result=excecuteLine(line, env);
            std::cout << result->toString() << std::endl;

        } catch (std::runtime_error &e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

void FILEmode(std::string filename){//文件模式
    auto env = EvalEnv::createGlobal();
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: cannot open file " << filename << std::endl;
        exit(1);
    }
    std::cout<<"file mode: "+filename<<std::endl;
    while(true) {
        try {
            std::string line = parseCode(file);
//debug            std::cout<<line<<std::endl;
            auto result=excecuteLine(line, env);
//debug            std::cout<<"file line run: "+result->toString()<<std::endl;
        } catch (std::runtime_error &e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}