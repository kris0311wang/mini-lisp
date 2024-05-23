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
    explicit Parser(std::deque<TokenPtr> tokens);;
    ValuePtr parse();
    ValuePtr parseTails() ;
};
#endif //MINI_LISP_PARSER_H
