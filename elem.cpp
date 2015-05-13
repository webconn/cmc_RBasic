#include "include/elem.h"

using namespace RBasic;

Elem& Elem::operator+=(const Elem &e)
{
        if (type == VAR_STRING || e.type == VAR_STRING)
                throw TypeException(Token(TOKEN_PLUS));

        // NULL ariphmetics - become NULL
        if (type == VAR_NULL || e.type == VAR_NULL) {
                type = VAR_NULL;
                return *this;
        }

        // bool ariphmetics - TRUE = 1, FALSE = 0
        if (type == VAR_LOGICAL) {
                type = VAR_NUMBER;
                num = bl ? 1.0 : 0.0;
        }

        if (e.type == VAR_LOGICAL) {
                num += e.bl ? 1.0 : 0.0;
        }

        if (type == VAR_NUMBER && e.type == VAR_NUMBER) {
                num += e.num;
        }

        return *this;
}
