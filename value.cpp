#include "include/rbasic.h"

RBasic::Value::Value(const RBasic::Variable &var)
{
        *this = var.getValue();
}

/**
 * @brief Generate value as range
 */
RBasic::Value::Value(double start, double end):
        type(VAR_NUMBER)
{
        if (start < end) {
                for (; start <= end; start += 1.0) {
                        elems.push_back(start);
                }
        } else {
                for (; start >= end; start -= 1.0) {
                        elems.push_back(start);
                }
        }
}

std::string RBasic::Value::getTypeName() const
{
        if (type == VAR_NULL) {
                return "NULL";
        } else if (type == VAR_NUMBER) {
                return "number";
        } else if (type == VAR_STRING) {
                return "character";
        } else if (type == VAR_LOGICAL) {
                return "logical";
        } else {
                return "unknown";
        }
}

std::ostream& operator<<(std::ostream &out, RBasic::Value &val)
{
        if (val.size() == 0) {
                out << val.getTypeName() << "(0)";
        }

        for (unsigned int i = 0; i < val.size(); i++) {
                out << val[i] << ' ';
        }
        out << std::endl;

        return out;
}

RBasic::Elem& RBasic::Value::operator[](unsigned int index)
{
        if (elems.size() <= index)
                throw OutOfRangeException();
        else
                return elems[index];
}

const RBasic::Elem& RBasic::Value::operator[](unsigned int index) const
{
        static const RBasic::Elem nul = Elem();

        if (elems.size() <= index)
                return nul;
        else
                return elems[index];
}

