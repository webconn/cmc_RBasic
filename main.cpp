#include <iostream>
#include <list>

#include "include/lex.h"

using namespace std;

int main()
{
        list<Token> *lst = lex_parse(cin);

        while (!lst->empty()) {
                cout << lst->front() << ' ';
                lst->pop_front();
        }

        delete lst;
}
