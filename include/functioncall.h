#ifndef INCLUDE_FUNCTIONCALL_H
#define INCLUDE_FUNCTIONCALL_H

#include "rbasic.h"

#include "value.h"

#include <map>
#include <vector>
#include <string>
#include <exception>
#include <sstream>

namespace RBasic {
        
        class Argument {
        public:
                bool named;
                std::string name;
                Value val;

                Argument(const Value &_val): named(false), val(_val) {}
                Argument(const std::string &_name, const Value &_val): 
                        named(true), name(_name), val(_val) {}
        };

        class NoSuchFunctionException {};
        class WrongArgumentsException {};

        typedef std::vector<Argument> ArgList;
        typedef Value (*Function)(const ArgList &);

        Value FunctionCall(const std::string &name, const ArgList &args);
        void addFunction(const std::string &name, Function func);
};

#endif
