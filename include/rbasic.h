#ifndef INCLUDE_RBASIC_H
#define INCLUDE_RBASIC_H

/**
 * @file
 * @brief RBasic core functions and data structures
 * @author Maslov Nikita
 */

#include <map>
#include <ostream>
#include <vector>
#include <string>

#include "lex.h"

#include "common.h"
#include "elem.h"
#include "value.h"
#include "variable.h"
#include "functioncall.h"

namespace RBasic {

        Value Operation(const Token &t, const Value &exp1, const Value &exp2);
        Value UnaryOperation(const Token &op, const Value &_e);

        Variable getVariable(const std::string &name);
        Variable getVariable(const std::string &name, const Value &index);
        Value getValue(const std::string &name);
        Value getValue(const Variable &var);
        void setVariable(const std::string &name, const Value &value);
        void setVariable(Variable &var, const Value &value);
};

std::ostream& operator<<(std::ostream &out, RBasic::Elem &val);
std::ostream& operator<<(std::ostream &out, RBasic::Value &val);

#endif
