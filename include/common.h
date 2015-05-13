#ifndef INCLUDE_COMMON_H
#define INCLUDE_COMMON_H

/**
 * @file
 * @brief Common definitions for RBasic core
 * @author Nikita <webconn> Maslov
a */

#include "lex.h"

namespace RBasic {

        enum var_type {
                VAR_NUMBER,
                VAR_LOGICAL,
                VAR_STRING,
                VAR_NULL
        };

        class NoVariableException {};
        class OutOfRangeException {};
        class TypeException {
        public:
                TypeException(const Token &t) {}
        };

};

#endif
