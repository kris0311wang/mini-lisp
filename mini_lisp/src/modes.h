//
// Created by 王愉博 on 24-5-29.
//

#ifndef MINI_LISP_MODES_H
#define MINI_LISP_MODES_H
#include <iostream>
class Value;
class EvalEnv;
using ValuePtr= std::shared_ptr<Value>;
ValuePtr excecuteLine(const std::string &line, std::shared_ptr<EvalEnv> &env);
bool containContent(std::string line);
std::string parseCode(std::istream& is,bool REPL=false);
void REPLmode();
void FILEmode(std::string filename);

#endif //MINI_LISP_MODES_H
