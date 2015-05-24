#include "include/rbasic.h"

#include <map>

static std::map<std::string, RBasic::Value> vars;

RBasic::Value RBasic::getValue(const std::string &name)
{
        if (vars.find(name) == vars.end()) { // no such variable
                throw RBasic::NoVariableException();
        }

        return vars[name];
}

RBasic::Value RBasic::getValue(const RBasic::Variable &var)
{
        return var.getValue();
}

RBasic::Variable RBasic::getVariable(const std::string &name)
{
        std::map<std::string, RBasic::Value>::iterator it = vars.find(name);
        
        if (it == vars.end()) { // no such variable
                vars[name] = Value();
                return Variable(vars.find(name)->second);
                //throw RBasic::NoVariableException();
        }

        return Variable(it->second);
}

RBasic::Variable RBasic::getVariable(const std::string &name, const RBasic::Value &index)
{
        std::map<std::string, RBasic::Value>::iterator it = vars.find(name);

        if (it == vars.end()) { // no such variable
                vars[name] = Value();
                return Variable(vars.find(name)->second);
        }

        return Variable(it->second, index);
}

void RBasic::setVariable(const std::string &name, const RBasic::Value &value)
{
        vars[name] = value;
}

void RBasic::setVariable(RBasic::Variable &var, const RBasic::Value &value)
{
        var = value;
}

std::ostream& operator<<(std::ostream &out, RBasic::Elem &val)
{
        if (val.type == RBasic::VAR_LOGICAL) {
                if (val.bl)
                        out << "TRUE";
                else
                        out << "FALSE";
        } else if (val.type == RBasic::VAR_NUMBER) {
                out << val.num;               
        } else if (val.type == RBasic::VAR_NULL) {
                out << "NULL";
        } else { // string
                out << val.str;
        }

        return out;
}

RBasic::Value RBasic::Operation(const Token &op, const RBasic::Value &_e1, const RBasic::Value &e2)
{
        RBasic::Value e1 = _e1;
        if (op.type == TOKEN_RANGE) { // generate range
                if (e1.getType() != VAR_NUMBER || e1.size() != 1 ||
                    e2.getType() != VAR_NUMBER || e2.size() != 1) {
                        throw TypeException(op);
                }

                return RBasic::Value(e1[0], e2[0]);
        } else if (op.type == TOKEN_PLUS) { // sum two values
                return e1 + e2;               
        } else if (op.type == TOKEN_MINUS) { // sub two values
                return e1 - e2;
        } else if (op.type == TOKEN_MUL) { // multiply
                return e1 * e2;
        } else if (op.type == TOKEN_DIV) { // divide
                return e1 / e2;
        } else if (op.type == TOKEN_L) { // less
                return e1 < e2;
        } else if (op.type == TOKEN_G) { // greater
                return e1 > e2;
        } else if (op.type == TOKEN_LE) { // less or equal
                return e1 <= e2;
        } else if (op.type == TOKEN_GE) { // greater or equal
                return e1 >= e2;
        } else if (op.type == TOKEN_EQ) { // equal
                return e1 == e2;
        } else if (op.type == TOKEN_NOT_EQ) { // not equal
                return e1 != e2;
        }

        return RBasic::Value();
}
