#ifndef INCLUDE_BUILTIN_H
#define INCLUDE_BUILTIN_H

#include "rbasic.h"

namespace BuiltIns {

        RBasic::Value c(const RBasic::ArgList &args);
        RBasic::Value mode(const RBasic::ArgList &args);
        RBasic::Value length(const RBasic::ArgList &args);

};

#endif
