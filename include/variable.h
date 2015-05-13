#ifndef INCLUDE_VARIABLE_H
#define INCLUDE_VARIABLE_H

/**
 * @file
 * @brief Variable class definition
 * @author Nikita <webconn> Maslov
 */

#include "rbasic.h"

#include "value.h"
#include "elem.h"

namespace RBasic {

        class Variable {
                
                Value *origin;
                std::vector<Elem *> lvalues;
                var_type type;

        public:
                Variable(): type(VAR_NULL) {}
                Variable(Value &val);
                Variable(Value &val, const Value &index);

                unsigned int size() const { return lvalues.size(); }

                Value& operator=(const Value &v);
                Value& operator=(const Elem &e);

                Value& getValue();
                const Value& getValue() const;

                operator Value() const { return getValue(); }
        };

};

#endif
