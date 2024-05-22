//
// Created by 王愉博 on 24-5-23.
//

#ifndef MINI_LISP_PARSER_H
#define MINI_LISP_PARSER_H
#include"value.h"
#include"token.h"
#include <deque>
class Parser {
    std::deque<TokenPtr> tokens;
public:
    Parser(std::deque<TokenPtr> tokens): tokens(std::move(tokens)) {};
    ValuePtr parse();
    ValuePtr parseTails() ;
};
#endif //MINI_LISP_PARSER_H
