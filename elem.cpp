#include "include/elem.h"

using namespace RBasic;

Elem& Elem::operator!()
{
        if (type == VAR_NULL) {
                return *this; // no change
        }

        if (type == VAR_STRING) {
                bl = str.length() == 0;
        } else if (type == VAR_NUMBER) {
                bl = num == 0;
        } else if (type == VAR_LOGICAL) {
                bl = !bl;
        }
        type = VAR_LOGICAL;

        return *this;
}

Elem& Elem::operator-()
{
        if (type == VAR_NULL) {
                return *this; // no change
        }

        if (type == VAR_STRING) {
                throw TypeException(Token(TOKEN_UNMINUS));
        } else if (type == VAR_NUMBER) {
                num = -num;
        } else if (type == VAR_LOGICAL) {
                num = bl ? -1.0 : 0.0;
        }

        type = VAR_NUMBER;

        return *this;
}

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

Elem& Elem::operator-=(const Elem &e)
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
                num -= e.bl ? 1.0 : 0.0;
        }

        if (type == VAR_NUMBER && e.type == VAR_NUMBER) {
                num -= e.num;
        }

        return *this;
}

Elem& Elem::operator*=(const Elem &e)
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
                num *= e.bl ? 1.0 : 0.0;
        }

        if (type == VAR_NUMBER && e.type == VAR_NUMBER) {
                num *= e.num;
        }

        return *this;
}

Elem& Elem::operator/=(const Elem &e)
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
                num /= e.bl ? 1.0 : 0.0;
        }

        if (type == VAR_NUMBER && e.type == VAR_NUMBER) {
                num /= e.num;
        }

        return *this;
}

Elem& Elem::operator|=(const Elem &e)
{
        if (type == VAR_STRING || e.type == VAR_STRING)
                throw TypeException(Token(TOKEN_PLUS));

        // NULL ariphmetics - TRUE if someone is TRUE, NULL otherwise
        if (type == VAR_NULL || e.type == VAR_NULL) {
                if ((type == VAR_LOGICAL && bl) || (e.type == VAR_LOGICAL && e.bl)) {
                        type = VAR_LOGICAL;
                        bl = true;
                } else {
                        type = VAR_NULL;
                }
                return *this;
        }

        // bool ariphmetics - TRUE = 1, FALSE = 0
        if (type == VAR_NUMBER) {
                type = VAR_LOGICAL;
                bl = num != 0;
        }

        if (e.type == VAR_NUMBER) {
                bl |= e.num != 0;
        }

        if (type == VAR_LOGICAL && e.type == VAR_LOGICAL) {
                bl |= e.bl;
        }

        return *this;
}

Elem& Elem::operator&=(const Elem &e)
{
        if (type == VAR_STRING || e.type == VAR_STRING)
                throw TypeException(Token(TOKEN_PLUS));

        // NULL ariphmetics - FALSE if someone is FALSE, NULL otherwise
        if (type == VAR_NULL || e.type == VAR_NULL) {
                if ((type == VAR_LOGICAL && !bl) || (e.type == VAR_LOGICAL && !e.bl)) {
                        type = VAR_LOGICAL;
                        bl = false;
                } else {
                        type = VAR_NULL;
                }
                return *this;
        }

        // bool ariphmetics - TRUE = 1, FALSE = 0
        if (type == VAR_NUMBER) {
                type = VAR_LOGICAL;
                bl = num != 0;
        }

        if (e.type == VAR_NUMBER) {
                bl &= e.num != 0;
        }

        if (type == VAR_LOGICAL && e.type == VAR_LOGICAL) {
                bl &= e.bl;
        }

        return *this;
}

std::string Elem::toString() const
{
        if (type == VAR_LOGICAL) {
                if (bl) {
                        return "TRUE";
                } else {
                        return "FALSE";
                }
        } else if (type == VAR_NUMBER) {
                std::string ret;
                std::stringstream str;

                str << num;
                str >> ret;

                return ret;
        } else if (type == VAR_STRING) {
                return str;
        }

        return "ERROR";
}

Elem Elem::operator>(const Elem &el) const
{
        if (el.type == VAR_NULL || type == VAR_NULL) {
                return Elem(); // NULL
        }

        // numbers
        if (type == VAR_NUMBER && el.type == VAR_NUMBER) {
                return Elem(num > el.num, true);
        }

        // strings
        if (type == VAR_STRING) {
                std::string sec = el.toString();
                return Elem(str > sec, true);
        }

        if (el.type == VAR_STRING) {
                std::string first = toString();
                return Elem(first > el.str, true);
        }

        return Elem(); // NULL
}

Elem Elem::operator<(const Elem &el) const
{
        if (el.type == VAR_NULL || type == VAR_NULL) {
                return Elem(); // NULL
        }

        // numbers
        if (type == VAR_NUMBER && el.type == VAR_NUMBER) {
                return Elem(num < el.num, true);
        }

        // strings
        if (type == VAR_STRING) {
                std::string sec = el.toString();
                return Elem(str < sec, true);
        }

        if (el.type == VAR_STRING) {
                std::string first = toString();
                return Elem(first < el.str, true);
        }

        return Elem(); // NULL
}

Elem Elem::operator>=(const Elem &el) const
{
        if (el.type == VAR_NULL || type == VAR_NULL) {
                return Elem(); // NULL
        }

        // numbers
        if (type == VAR_NUMBER && el.type == VAR_NUMBER) {
                return Elem(num >= el.num, true);
        }

        // strings
        if (type == VAR_STRING) {
                std::string sec = el.toString();
                return Elem(str >= sec, true);
        }

        if (el.type == VAR_STRING) {
                std::string first = toString();
                return Elem(first >= el.str, true);
        }

        return Elem(); // NULL
}

Elem Elem::operator<=(const Elem &el) const
{
        if (el.type == VAR_NULL || type == VAR_NULL) {
                return Elem(); // NULL
        }

        // numbers
        if (type == VAR_NUMBER && el.type == VAR_NUMBER) {
                return Elem(num <= el.num, true);
        }

        // strings
        if (type == VAR_STRING) {
                std::string sec = el.toString();
                return Elem(str <= sec, true);
        }

        if (el.type == VAR_STRING) {
                std::string first = toString();
                return Elem(first <= el.str, true);
        }

        return Elem(); // NULL
}

Elem Elem::operator==(const Elem &el) const
{
        if (el.type == VAR_NULL || type == VAR_NULL) {
                return Elem(); // NULL
        }

        if (type == VAR_LOGICAL && el.type == VAR_LOGICAL) {
                return Elem(bl == el.bl, true);
        }

        // numbers
        if (type == VAR_NUMBER && el.type == VAR_NUMBER) {
                return Elem(num == el.num, true);
        }

        // strings
        if (type == VAR_STRING) {
                std::string sec = el.toString();
                return Elem(str == sec, true);
        }

        if (el.type == VAR_STRING) {
                std::string first = toString();
                return Elem(first == el.str, true);
        }

        return Elem(); // NULL
}

Elem Elem::operator!=(const Elem &el) const
{
        if (el.type == VAR_NULL || type == VAR_NULL) {
                return Elem(); // NULL
        }

        if (type == VAR_LOGICAL && el.type == VAR_LOGICAL) {
                return Elem(bl != el.bl, true);
        }

        // numbers
        if (type == VAR_NUMBER && el.type == VAR_NUMBER) {
                return Elem(num != el.num, true);
        }

        // strings
        if (type == VAR_STRING) {
                std::string sec = el.toString();
                return Elem(str != sec, true);
        }

        if (el.type == VAR_STRING) {
                std::string first = toString();
                return Elem(first != el.str, true);
        }

        return Elem(); // NULL
}
