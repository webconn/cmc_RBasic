#include "include/lex.h"
#include <cstring>
#include <cctype>



/**
 * Get token from input stream
 */

Token stream_token_iterator::getToken()
{
        Token t = getTokenRaw();

        if (t.type == TOKEN_END) {
                // wait for end of expression
                if (until.type != TOKEN_END) {
                        while (t.type == TOKEN_END) {
                                if (in == std::cin) {
                                        std::cout << "+ ";
                                }
                                t = getTokenRaw();
                        }

                        if (t == until) {
                                t.type = TOKEN_END;
                        }
                }
        }

        return t;
}

Token stream_token_iterator::getTokenRaw()
{
        token_type t;

        Token ret;

        char s = in.get();

        // Skip spaces
        while (!in.eof() && isblank(s)) s = in.get();

        // Symbols
        if (in.eof()) {
                t = TOKEN_EOF;
        } else if (s == ';') {
                t = TOKEN_END;
        } else if (s == '\n' || s == '\0' || s == '#') {
                t = TOKEN_END;
        } else if (s == ',') {
                t = TOKEN_COMMA;
        } else if (s == '(') {
                t = TOKEN_OPBR;
        } else if (s == ')') {
                t = TOKEN_CLBR;
        } else if (s == '[') {
                t = TOKEN_OPIND;
        } else if (s == ']') {
                t = TOKEN_CLIND;
        } else if (s == '{') {
                t = TOKEN_OPBLK;
        } else if (s == '}') {
                t = TOKEN_CLBLK;
        } else if (s == '&') {
                t = TOKEN_AND;
        } else if (s == '|') {
                t = TOKEN_OR;
        } else if (s == ':') {
                t = TOKEN_RANGE;
        } else if (s == '+') {
                t = TOKEN_PLUS;
        } else if (s == '-') {
                t = TOKEN_MINUS;
        } else if (s == '*') {
                t = TOKEN_MUL;
        } else if (s == '/') {
                t = TOKEN_DIV;
        } else if (s == '<') {
                s = in.get();
                if (s == '-') {
                        t = TOKEN_ASSIGN;
                } else if (s == '=') {
                        t = TOKEN_LE;
                } else {
                        t = TOKEN_L;
                        in.unget();
                }
        } else if (s == '>') {
                s = in.get();
                if (s == '=') {
                        t = TOKEN_GE;
                } else {
                        t = TOKEN_G;
                        in.unget();
                }
        } else if (s == '=') {
                s = in.get();
                if (s == '=') {
                        t = TOKEN_EQ;
                } else {
                        t = TOKEN_ERROR;
                        in.unget();
                }
        } else if (s == '!') {
                s = in.get();
                if (s == '=') {
                        t = TOKEN_NOT_EQ;
                } else {
                        t = TOKEN_NOT;
                        in.unget();
                }
        } else if (s == '"') { // string
                // try to get whole string
                std::string buf;
                
                s = in.get();
                
                while (s != '"') {
                        if (s == '\\') { // escape
                                s = in.get();

                                if (s == 'n') { // new line
                                        buf += '\n';
                                } else if (s == 't') {
                                        buf += '\t';
                                } else {
                                        buf += s;
                                }
                        } else {
                                buf += s;
                        }
                        s = in.get();
                }

                ret = Token(buf); // string
        } else if (isdigit(s)) { // number
                in.unget();
                
                double buf;
                in >> buf;

                ret = Token(buf);
        } else if (isalpha(s)) { // identifier or word
                std::string buf;
                
                while (isalnum(s) || s == '_' || s == '.') {
                        buf += s;
                        s = in.get();
                }
                in.unget();

                if (buf == "NULL") {
                        t = TOKEN_NULL;
                } else if (buf == "TRUE") {
                        t = TOKEN_TRUE;
                } else if (buf == "FALSE") {
                        t = TOKEN_FALSE;
                } else {
                        ret = Token(buf, true); // identifier
                }
        } else {
                t = TOKEN_ERROR;
        }
        
        if (ret.type == TOKEN_END) {
                ret.type = t;
        }

        return ret;
}

/**
 * Get token from list
 */
Token list_token_iterator::getToken()
{
        return *(it++);
}



const Token& token_iterator::operator*()
{
        if (empty) {
                empty = false;
                getPrevious = false;
                current = getToken();
        }
        
        if (getPrevious) {
                return prev;
        } else {
                return current;
        }
}

/**
 * Switch to next token in interator
 */
token_iterator& token_iterator::operator++(int dummy)
{
        if (empty) {
                empty = false;
                getPrevious = false;
                current = getToken();
        }

        if (getPrevious) { // if last token was returned back, just shift
                getPrevious = false;
        } else { // get brand new token
                prev = current;
                current = getToken();
        }

        return *this;
}

token_iterator& token_iterator::operator--(int dummy)
{
        // if iterator is empty, nothing to shift back
        if (empty)
                throw EmptyTokenIteratorException();

        // just require previous token
        getPrevious = true;

        return *this;
}

/**
 * @brief Get weight of the operator
 * Less number - less weight - less priority
 * More number - more weight - higher priority
 * 
 * @return unsigned int Weight
 */
unsigned int Token::weight() const
{
        switch (type) {
                case TOKEN_AND:
                case TOKEN_OR:
                        return 1;
                case TOKEN_NOT:
                        return 2;
                case TOKEN_L:
                case TOKEN_LE:
                case TOKEN_G:
                case TOKEN_GE:
                case TOKEN_EQ:
                case TOKEN_NOT_EQ:
                        return 3;
                case TOKEN_PLUS:
                case TOKEN_MINUS:
                        return 4;
                case TOKEN_MUL:
                case TOKEN_DIV:
                        return 5;
                case TOKEN_RANGE:
                        return 6;
                default:
                        return 0;
        }
}

bool Token::operator==(const Token &t) const
{
        if (t.type != type) {
                return false;
        } else {
                if (type == TOKEN_NUMBER && t.dbl != dbl) {
                        return false;
                } else if ((type == TOKEN_STRING || type == TOKEN_ID) && t.str != str) {
                        return false;
                } 
                
                return true;
        }
}

std::ostream& operator<<(std::ostream &in, const Token &t)
{
        if (t.type == TOKEN_NUMBER) {
                in << "number(" << t.dbl << ")";
        } else if (t.type == TOKEN_STRING) {
                in << "string(" << t.str << ")";
        } else if (t.type == TOKEN_ID) {
                in << "id(" << t.str << ")";
        } else if (t.type == TOKEN_ERROR) {
                in << "error_token";
        } else {
                in << "token(" << t.type << ")";
        }

        return in;
}
