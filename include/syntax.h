#ifndef INCLUDE_SYNTAX_H
#define INCLUDE_SYNTAX_H

#include <string>
#include <istream>
#include <list>
#include <cstdlib>
#include <vector>

#include "lex.h"
#include "rbasic.h"

namespace Grammar {

        bool Program(token_iterator &lst);
        RBasic::Value Expression(token_iterator &lst, const Token &until = Token());
        RBasic::Value Block(token_iterator &lst);
        RBasic::Value Exp1(token_iterator &lst, const Token &until = Token(), 
                        bool fail = false, bool add = false, 
                        const RBasic::Value &add_val = RBasic::Value(),
                        bool assign = false);
        RBasic::Value FunctionCall(token_iterator &lst);
        RBasic::Variable Variable(token_iterator &lst);
        RBasic::ArgList ArgList(token_iterator &it);

        bool Calculate(std::list<Token> &op_stack, std::list<RBasic::Value> &val_stack);

        class NotAVariableException {};
        class IsFunctionCallException: public NotAVariableException {};
        class NotAFunctionCallException {};
        class EmptyExpressionException {};
        class NoOperandException {
        public:
                NoOperandException(const Token &t) {}
        };
        class UnexpectedTokenException {};
};

bool syntax_parse(token_iterator &in);

#endif
