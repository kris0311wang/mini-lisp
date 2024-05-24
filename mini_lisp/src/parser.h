//
// Created by 王愉博 on 24-5-23.
//

#ifndef MINI_LISP_PARSER_H
#define MINI_LISP_PARSER_H

#include <deque>
class Token;
using TokenPtr = std::unique_ptr<Token>;
class Value;
using ValuePtr = std::shared_ptr<Value>;

class Parser {
    std::deque<TokenPtr> tokens;
public:
    explicit Parser(std::deque<TokenPtr> tokens);;

    ValuePtr parse();

    ValuePtr parseTails();
};

#endif //MINI_LISP_PARSER_H
