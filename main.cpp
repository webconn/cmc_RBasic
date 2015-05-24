#include <iostream>
#include <list>

#include "include/lex.h"
#include "include/syntax.h"

using namespace std;

int main()
{
        stream_token_iterator it(cin, Token(TOKEN_EOF));

        syntax_parse(it);
}
