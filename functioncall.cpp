#include "include/functioncall.h"

static std::map<std::string, RBasic::Function> functions;

RBasic::Value RBasic::FunctionCall(const std::string &str, const RBasic::ArgList &args)
{
        if (functions.find(str) == functions.end()) { // no such function
                throw NoSuchFunctionException();
        }

        return functions[str](args);
}

void RBasic::addFunction(const std::string &str, Function func)
{
        functions[str] = func;
}
