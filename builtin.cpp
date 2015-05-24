#include "include/builtin.h"

RBasic::Value BuiltIns::c(const RBasic::ArgList &args)
{
        RBasic::Value ret;

        for (unsigned int i = 0; i < args.size(); i++) {
                for (unsigned int j = 0; j < args[i].val.size(); j++) {
                        ret.push_back(args[i].val[j]);
                }
        }

        return ret;
}

RBasic::Value BuiltIns::length(const RBasic::ArgList &args)
{
        if (args.size() != 1) {
                throw RBasic::WrongArgumentsException();
        }

        return RBasic::Value(args[0].val.size());
}

RBasic::Value BuiltIns::mode(const RBasic::ArgList &args)
{
        if (args.size() != 1) {
                throw RBasic::WrongArgumentsException();
        }

        return RBasic::Value(args[0].val.getTypeName());
}
