#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
//
// Created by 王愉博 on 24-5-23.
//
#include "parser.h"
#include "error.h"
#include "token.h"
#include "value.h"

ValuePtr Parser::parse() {
    auto token = std::move(tokens.front());
    tokens.pop_front();
    if (token->getType() == TokenType::NUMERIC_LITERAL) {
        auto value = static_cast<NumericLiteralToken &>(*token).getValue();
        return std::make_shared<NumericValue>(value);
    } else if (token->getType() == TokenType::BOOLEAN_LITERAL) {
        auto value = static_cast<BooleanLiteralToken &>(*token).getValue();
        return std::make_shared<BooleanValue>(value);
    } else if (token->getType() == TokenType::STRING_LITERAL) {
        auto value = static_cast<StringLiteralToken &>(*token).getValue();
        return std::make_shared<StringValue>(value);
    } else if (token->getType() == TokenType::IDENTIFIER) {
        auto value = static_cast<IdentifierToken &>(*token).getName();
        return std::make_shared<SymbolValue>(value);
    } else if (token->getType() == TokenType::QUOTE) {
        return std::make_shared<PairValue>(
                std::make_shared<SymbolValue>("quote"),
                std::make_shared<PairValue>(
                        this->parse(),
                        std::make_shared<NilValue>()
                )
        );
    } else if (token->getType() == TokenType::QUASIQUOTE) {
        return std::make_shared<PairValue>(
                std::make_shared<SymbolValue>("quasiquote"),
                std::make_shared<PairValue>(
                        this->parse(),
                        std::make_shared<NilValue>()
                )
        );
    } else if (token->getType() == TokenType::UNQUOTE) {
        return std::make_shared<PairValue>(
                std::make_shared<SymbolValue>("unquote"),
                std::make_shared<PairValue>(
                        this->parse(),
                        std::make_shared<NilValue>()
                )
        );
    } else if (token->getType() == TokenType::LEFT_PAREN) {
        return this->parseTails();
    }
    throw SyntaxError("Unimplemented");
}

ValuePtr Parser::parseTails() {
    if (tokens.front()->getType() == TokenType::RIGHT_PAREN) {
        tokens.pop_front();//弹出这个词法标记;
        return std::make_shared<NilValue>();//return 空表;
    }
    auto car = parse();
    if (tokens.front()->getType() == TokenType::DOT) {
        tokens.pop_front();//弹出这个词法标记;
        auto cdr = parse();
        if (tokens.front()->getType() != TokenType::RIGHT_PAREN) {
            throw SyntaxError("Expecting a right parenthesis");
        }
        tokens.pop_front();//弹出这个词法标记;
        return std::make_shared<PairValue>(car, cdr);
    } else {
        auto cdr = this->parseTails();
        return std::make_shared<PairValue>(car, cdr);
    }
}

Parser::Parser(std::deque<TokenPtr> tokens) : tokens(std::move(tokens)) {}

#pragma clang diagnostic pop