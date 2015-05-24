#ifndef INCLUDE_VALUE_H
#define INCLUDE_VALUE_H

/**
 * @file
 * @brief Value class definition
 * @author Nikita <webconn> Maslov
 */

#include <vector>

#include "elem.h"

namespace RBasic {

        class Value {
                
                std::vector<Elem> elems;
                var_type type;

        public:

                Value() : type(VAR_NULL) {}
                Value(var_type t) : type(t) {}
                Value(const Elem &el) : type(el.type) { elems.push_back(el); }
                Value(double start, double end); // generate range

                unsigned int size() const { return elems.size(); }
                Elem& operator[](unsigned int index);
                const Elem& operator[](unsigned int index) const;
                Value operator[](const Value &index) const;

                void expand(unsigned int new_size);
                void trim(unsigned int new_size);

                var_type getType() const { return type; }
                std::string getTypeName() const;

                // TODO: convert types
                void push_back(const Elem &el) { 
                        if (type == VAR_NULL) type = el.type; 
                        elems.push_back(el); 
                }

                Value& operator+(const Value &val);
                Value& operator-(const Value &val);
                Value& operator*(const Value &val);
                Value& operator/(const Value &val);

                // TODO: make it const by creating copies
                Value operator>(const Value &val);
                Value operator<(const Value &val);
                Value operator>=(const Value &val);
                Value operator<=(const Value &val);
                Value operator==(const Value &val);
                Value operator!=(const Value &val);

        };
};

#endif
