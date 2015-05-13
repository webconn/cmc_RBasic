#include "include/rbasic.h"
#include <algorithm>

using namespace RBasic;

Variable::Variable(Value &val):
        origin(&val),
        type(val.getType())
{
        for (unsigned int i = 0; i < val.size(); i++) {
                lvalues.push_back(&val[i]);
        }
}

Variable::Variable(Value &val, const Value &index):
        origin(&val),
        type(val.getType())
{
        // Get existing elements
        unsigned int val_size = val.size();
        unsigned int in_size = index.size();

        unsigned int m = std::min(val_size, in_size);

        for (unsigned int i = 0; i < m; i++) {
                if (index[i % in_size]) {
                        lvalues.push_back(&val[i]);
                }
        }

        // Append new NULLs
        if (in_size > val_size) {
                for (unsigned int i = val_size; i < in_size; i++) {
                        val.push_back(Elem());
                        if (index[i]) {
                                lvalues.push_back(&val[i]);
                        }
                }
        }
}

Value& Variable::operator=(const Value &val)
{
        // TODO: check types

        // Set existing elements
        unsigned int lv_size = lvalues.size();
        unsigned int val_size = val.size();

        unsigned int m = std::min(lv_size, val_size);
        for (unsigned int i = 0; i < m; i++) {
                *(lvalues[i]) = val[i];
        }

        // Append new elements
        if (val_size > lv_size) {
                for (unsigned int i = lv_size; i < val_size; i++) {
                        origin->push_back(val[i]);
                        lvalues.push_back(&((*origin)[i]));
                }
        }

        return *origin;
}

Value& Variable::getValue() 
{
        // TODO: this is a shit
        Value *val = new Value();

        for (unsigned int i = 0; i < lvalues.size(); i++) {
                val->push_back(*(lvalues[i]));
        }

        return *val;
}

const Value& Variable::getValue() const
{
        Value *val = new Value();

        for (unsigned int i = 0; i < lvalues.size(); i++) {
                val->push_back(*(lvalues[i]));
        }

        return *val;
}
