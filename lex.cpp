#include "include/lex.h"
#include <cstring>
#include <cctype>

static Token *get_token(std::istream &in);

token_list *lex_parse(std::istream &input)
{
        std::list<Token> *lst = new std::list<Token>;
        
        Token *t;

        while ((t = get_token(input)) != NULL) {
                lst->push_back(*t);
                delete t;
        }

        return lst;
}

static Token *get_token(std::istream &in)
{
        token_type t;

        static bool end = false;

        if (end == true) {
                end = false;
                return NULL;
        }

        char s = in.get();

        // Skip spaces
        while (isblank(s)) s = in.get();

        // Symbols
        if (s == ';' || s == '\n' || s == '\0' || s == '#') {
                t = TOKEN_END;
        } else if (s == ',') {
                t = TOKEN_COMMA;
        } else if (s == '(') {
                t = TOKEN_OPBR;
        } else if (s == ')') {
                t = TOKEN_CLBR;
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

                return new Token(buf); // string
        } else if (isdigit(s)) { // number
                in.unget();
                
                double buf;
                in >> buf;

                return new Token(buf);
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
                        return new Token(buf, true); // identifier
                }
        } else {
                t = TOKEN_ERROR;
        }
        
        if (t == TOKEN_END) {
                end = true;
        }

        return new Token(t);
}

std::ostream& operator<<(std::ostream &in, Token &t)
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
