#ifndef INCLUDE_ELEM_H
#define INCLUDE_ELEM_H

/**
 * @file
 * @brief Definition of vector element type
 * @author Nikita <webconn> Maslov
 */

#include "common.h"

#include <string>
#include <sstream>

namespace RBasic {

        struct Elem {
                
                var_type type;

                bool bl;
                double num;
                std::string str;
                
                Elem(): type(VAR_NULL) {}
                Elem(bool _bl, bool is_bool): type(VAR_LOGICAL), bl(_bl) {}
                Elem(double _num): type(VAR_NUMBER), num(_num) {}
                Elem(const std::string &_str): type(VAR_STRING), str(_str) {}
                Elem(const char* _str): type(VAR_STRING), str(_str) {}
                //Elem(const Variable &var);

                void setBool(bool _bl) { type = VAR_LOGICAL; bl = _bl; }
                void set(double _num) { type = VAR_NUMBER; num = _num; }
                void set(const std::string &_str) { type = VAR_STRING; str = _str; }

                std::string toString() const;

                operator bool() const { return (type == VAR_LOGICAL && bl); }
                operator double() const { if (type == VAR_NUMBER) return num; else return 0.0; }

                Elem& operator+=(const Elem &el);
                Elem& operator-=(const Elem &el);
                Elem& operator*=(const Elem &el);
                Elem& operator/=(const Elem &el);

                Elem operator>(const Elem &el) const;
                Elem operator<(const Elem &el) const;
                Elem operator>=(const Elem &el) const;
                Elem operator<=(const Elem &el) const;
                Elem operator==(const Elem &el) const;
                Elem operator!=(const Elem &el) const;
        };

};

#endif
