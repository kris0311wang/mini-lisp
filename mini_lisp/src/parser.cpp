#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
//
// Created by 王愉博 on 24-5-23.
//
#include "parser.h"
#include "error.h"
#include "token.h"
#include "value.h"

ValuePtr Parser::parse(bool REPL) {//解析一个对象
    auto token = std::move(tokens.front());
    tokens.pop_front();
    if (token->getType() == TokenType::NUMERIC_LITERAL) {
        auto value = static_cast<NumericLiteralToken &>(*token).getValue();
        //以蓝色显示数字
        if (REPL)
            std::cout << "\033[34m" << value << "\033[0m"<<" ";
        return std::make_shared<NumericValue>(value);
    } else if (token->getType() == TokenType::BOOLEAN_LITERAL) {
        auto value = static_cast<BooleanLiteralToken &>(*token).getValue();
        //以黄色显示布尔值
        if (REPL)
            std::cout << "\033[33m" << (value ? "#t" : "#f") << "\033[0m"<<" ";
        return std::make_shared<BooleanValue>(value);
    } else if (token->getType() == TokenType::STRING_LITERAL) {
        auto value = static_cast<StringLiteralToken &>(*token).getValue();
        //以绿色显示字符串
        if (REPL)
            std::cout << "\033[32m" << value << "\033[0m"<<" ";
        return std::make_shared<StringValue>(value);
    } else if (token->getType() == TokenType::IDENTIFIER) {
        auto value = static_cast<IdentifierToken &>(*token).getName();
        //以紫色显示标识符
        if (REPL)
            std::cout << "\033[35m" << value << "\033[0m"<<" ";
        return std::make_shared<SymbolValue>(value);
    } else if (token->getType() == TokenType::QUOTE) {
        if(REPL)
            std::cout<<"'";
        return std::make_shared<PairValue>(
                std::make_shared<SymbolValue>("quote"),
                std::make_shared<PairValue>(
                        this->parse(REPL),
                        std::make_shared<NilValue>()
                )
        );
    } else if (token->getType() == TokenType::QUASIQUOTE) {
        if(REPL)
            std::cout<<"`";
        return std::make_shared<PairValue>(
                std::make_shared<SymbolValue>("quasiquote"),
                std::make_shared<PairValue>(
                        this->parse(REPL),
                        std::make_shared<NilValue>()
                )
        );
    } else if (token->getType() == TokenType::UNQUOTE) {
        if(REPL)
            std::cout<<",";
        return std::make_shared<PairValue>(
                std::make_shared<SymbolValue>("unquote"),
                std::make_shared<PairValue>(
                        this->parse(REPL),
                        std::make_shared<NilValue>()
                )
        );
    } else if (token->getType() == TokenType::LEFT_PAREN) {
        if(REPL)
            //红色显示左括号
            std::cout << "\033[31m" << "(" << "\033[0m"<<" ";
        return this->parseTails(REPL);
    }
    throw SyntaxError("Unimplemented");
}

ValuePtr Parser::parseTails(bool REPL) {
    if (tokens.front()->getType() == TokenType::RIGHT_PAREN) {
        if(REPL)
            //红色显示右括号
            std::cout << "\033[31m" << ")" << "\033[0m"<<" ";
        tokens.pop_front();//弹出这个词法标记;
        return std::make_shared<NilValue>();//return 空表;
    }
    auto car = parse(REPL);
    if (tokens.front()->getType() == TokenType::DOT) {
        if(REPL)
            std::cout << ".";
        tokens.pop_front();//弹出这个词法标记;
        auto cdr = parse(REPL);
        if (tokens.front()->getType() != TokenType::RIGHT_PAREN) {
            if(REPL)
                std::cout << "\033[31m" << ")" << "\033[0m"<<" ";
            throw SyntaxError("Expecting a right parenthesis");
        }
        tokens.pop_front();//弹出这个词法标记;
        return std::make_shared<PairValue>(car, cdr);
    } else {
        auto cdr = this->parseTails(REPL);
        return std::make_shared<PairValue>(car, cdr);
    }
}

Parser::Parser(std::deque<TokenPtr> tokens) : tokens(std::move(tokens)) {}

#pragma clang diagnostic pop