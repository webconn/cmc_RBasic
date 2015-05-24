#include "include/rbasic.h"
#include <algorithm>

using namespace RBasic;

Variable::Variable(Value &val):
        origin(&val),
        type(val.getType()),
        original(true)
{
        for (unsigned int i = 0; i < val.size(); i++) {
                lvalues.push_back(&val[i]);
        }
}

Variable::Variable(Value &val, const Value &index):
        origin(&val),
        type(val.getType()),
        original(false)
{
        if (index.getType() == VAR_LOGICAL) {
                // Get existing elements
                unsigned int val_size = val.size();
                unsigned int in_size = index.size();

                // Append new NULLs
                if (in_size > val_size) {
                        for (unsigned int i = val_size; i < in_size; i++) {
                                val.push_back(Elem());
                        }
                        
                        val_size = in_size;
                }

                // Create lvalues table
                for (unsigned int i = 0; i < val_size; i++) {
                        if (index[i % in_size]) {
                                lvalues.push_back(&val[i]);
                        }
                }

        } else if (index.getType() == VAR_NUMBER) { // need to get index table

                unsigned int in_size = index.size();

                for (unsigned int i = 0; i < index.size(); i++) {
                        // resize values table if required
                        if (static_cast<double>(index[i % in_size]) > val.size()) {
                                val.expand(static_cast<double>(index[i]));
                        }

                        // numbers starting from 1
                        lvalues.push_back(&val[static_cast<double>(index[i % in_size]) - 1]);
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
                *(lvalues[i]) = val[i % val_size];
        }

        // Append new elements
        if (val_size > lv_size) {
                for (unsigned int i = lv_size; i < val_size; i++) {
                        origin->push_back(val[i]);
                        lvalues.push_back(&((*origin)[i]));
                }
        }

        // Remove unnecessary elems
        else if (val_size < lv_size) {
                if (original) {
                        origin->trim(val_size);
                        lvalues.resize(val_size);
                } else {
                        for (unsigned int i = m; i < lv_size; i++) {
                                *(lvalues[i]) = val[i % val_size];
                        }
                }
        }

        return *origin;
}

Value Variable::getValue() const
{
        Value val;

        for (unsigned int i = 0; i < lvalues.size(); i++) {
                val.push_back(*(lvalues[i]));
        }

        return val;
}
