#include <iostream>
#include <list>

#include "include/lex.h"
#include "include/syntax.h"

#include "include/rbasic.h"
#include "include/builtin.h"

using namespace std;

int main()
{
        stream_token_iterator it(cin, Token(TOKEN_EOF));

        RBasic::addFunction("c", BuiltIns::c);
        RBasic::addFunction("mode", BuiltIns::mode);
        RBasic::addFunction("length", BuiltIns::length);

        syntax_parse(it);
}
