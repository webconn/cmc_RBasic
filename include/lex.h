#ifndef INCLUDE_LEX_H
#define INCLUDE_LEX_H

#include <string>
#include <iostream>
#include <list>
#include <set>

enum token_type {
        // Basic types and values
        TOKEN_NUMBER,
        TOKEN_STRING,
        TOKEN_ID,
        TOKEN_NULL,
        TOKEN_TRUE,
        TOKEN_FALSE,

        TOKEN_OPBR, // (
        TOKEN_CLBR, // )

        TOKEN_OPBLK, // {
        TOKEN_CLBLK, // }

        TOKEN_OPIND, // [
        TOKEN_CLIND, // ]

        TOKEN_COMMA,

        // Operators
        TOKEN_ASSIGN,
        TOKEN_AND,
        TOKEN_OR,
        TOKEN_NOT,
        
        TOKEN_G,
        TOKEN_L,
        TOKEN_GE,
        TOKEN_LE,
        TOKEN_EQ, // ==
        TOKEN_NOT_EQ, // !=
        TOKEN_SEQ, // =

        TOKEN_PLUS,
        TOKEN_MINUS,
        TOKEN_MUL,
        TOKEN_DIV,
        
        TOKEN_RANGE,

        TOKEN_END,
        TOKEN_ENDS,
        TOKEN_ERROR,
        TOKEN_EOF,
};

/**
 * @brief Describes a single token
 */
class Token {

public:

        token_type type;

        double dbl;
        std::string str;

        Token(): type(TOKEN_END) {}
        Token(token_type _type): type(_type) {}
        Token(const std::string &_str): type(TOKEN_STRING), str(_str) {}
        Token(const std::string &_str, bool id): type(TOKEN_ID), str(_str) {}
        Token(double _dbl): type(TOKEN_NUMBER), dbl(_dbl) {}

        bool operator==(const Token &t) const;

        unsigned int weight() const;
        bool isOperator() const { return weight() > 0; };
};

/**
 * @brief Token iterator
 * Implements token reading mechanism
 */
class token_iterator 
{
        bool empty;
        bool getPrevious;
protected:

        Token until;
        Token prev;
        Token current;
        
        virtual Token getToken() = 0;
        token_iterator(const Token &_until): empty(true), getPrevious(false), 
                                                until(_until) {}

public:
        token_iterator& operator++(int);
        token_iterator& operator--(int);

        const Token& operator*();
        
        const Token* operator->() {
                return &operator*();
        }
        
        void readUntil(const Token &t) { until = t; }
        bool eof() { return operator*() == until; }
};

/**
 * Token iterator based on input bytestream
 */
class stream_token_iterator: public token_iterator
{
        std::istream &in;
        Token getToken();

public:
        stream_token_iterator(std::istream &st, const Token &end = Token()): 
                token_iterator(end), in(st) {}
};

/**
 * Empty token iterator exception
 */
class EmptyTokenIteratorException {};

/**
 * Token list definition - based on std::list container
 */
typedef std::list<Token> token_list;

/**
 * Token iterator based on token list
 */
class list_token_iterator: public token_iterator
{
        const token_list &lst;
        token_list::const_iterator it;

        Token getToken();
public:
        list_token_iterator(const token_list &_lst, const Token &end = Token()): 
                token_iterator(end), lst(_lst), it(_lst.begin()) {}
};


std::ostream& operator<<(std::ostream &in, Token &t);
token_list *lex_parse(std::istream &input, const Token &until = Token());

#endif
