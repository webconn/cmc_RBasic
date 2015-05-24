#include <iostream>
#include <fstream>
#include <cstdio>
#include <list>

#include "include/lex.h"
#include "include/syntax.h"

#include "include/rbasic.h"
#include "include/builtin.h"

using namespace std;

int main(int argc, char *argv[])
{
        if (argc == 2) {
                if (!freopen(argv[1], "r", stdin)) {
                        cerr << "Error opening file " << argv[1] << '\n';
                        perror("");
                        return 1;
                }
        }
        
        stream_token_iterator it(cin, argc != 2, Token(TOKEN_EOF));

        RBasic::addFunction("c", BuiltIns::c);
        RBasic::addFunction("mode", BuiltIns::mode);
        RBasic::addFunction("length", BuiltIns::length);

        syntax_parse(it);
}
