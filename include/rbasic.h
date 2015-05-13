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

namespace RBasic {

        struct Value;
        class Variable;

        enum var_type {
                VAR_NUMBER,
                VAR_LOGICAL,
                VAR_STRING,
                VAR_NULL
        };

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
                Elem(const Variable &var);

                void setBool(bool _bl) { type = VAR_LOGICAL; bl = _bl; }
                void set(double _num) { type = VAR_NUMBER; num = _num; }
                void set(const std::string &_str) { type = VAR_STRING; str = _str; }

                operator bool() const { return (type == VAR_LOGICAL && bl); }
                operator double() const { if (type == VAR_NUMBER) return num; else return 0.0; }

                Elem& operator+(const Elem &el);
                Elem& operator-(const Elem &el);
                Elem& operator*(const Elem &el);
                Elem& operator/(const Elem &el);
        };

        class Value {
                
                std::vector<Elem> elems;
                var_type type;

        public:

                Value() : type(VAR_NULL) {}
                Value(const Elem &el) : type(el.type) { elems.push_back(el); }
                Value(const Variable &var);
                Value(double start, double end); // generate range

                unsigned int size() const { return elems.size(); }
                Elem& operator[](unsigned int index);
                const Elem& operator[](unsigned int index) const;
                Value operator[](const Value &index) const;

                void expand(unsigned int new_size);

                var_type getType() const { return type; }
                std::string getTypeName() const;

                void push_back(const Elem &el) { elems.push_back(el); }

                Value& operator+(const Value &val);
                Value& operator-(const Value &val);
                Value& operator*(const Value &val);
                Value& operator/(const Value &val);

        };

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
        };


        class NoVariableException {};
        class OutOfRangeException {};
        class TypeException {
        public:
                TypeException(const Token &t) {}
        };

        Value Operation(const Token &t, const Value &exp1, const Value &exp2);

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
