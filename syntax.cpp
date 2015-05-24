#include "include/syntax.h"

//static bool cont = false;
static Token until = Token();

/**
 * @brief Parse token stream using syntax analyzer
 *
 * Take bytestream, create token list and parse it
 *
 * @param std::stream in Input bytestream
 * @return bool True if stream parsed successfully, or false otherwise
 */
bool syntax_parse(token_iterator &t)
{
        // invoke grammar parser
        Grammar::Program(t);

        return true;
}

/**
 * @brief Parse whole program
 * @param token_iterator t Pointer to current token in list
 * @return bool True if parsed successfully, false otherwise
 */
bool Grammar::Program(token_iterator &t)
{
        while (!t.eof()) {
                Grammar::Expression(t);
                
                if (t->type == TOKEN_END || t->type == TOKEN_ENDS)
                        t++;
        }
        return true;
}

/**
 * @brief Parse expression from grammar
 * @param token_iterator t Pointer to current token in list
 * @return RBasic::Value Value of expression
 */
RBasic::Value Grammar::Expression(token_iterator& t, const Token &until)
{
        // context indicator - don't print the value if it is not a top level
        static bool context = true; // true is a top level 

        // save context
        bool old_context = context;
        context = false;

        // create list of assignment variables
        std::list<RBasic::Variable> vars;

        do {
                // we need to have variable first and then assignment statement
                RBasic::Variable var;
                try {
                        var = Grammar::Variable(t);
                } catch (...) { // it is not a variable...
                        break;
                }

                if (t->type == TOKEN_ASSIGN) {
                        vars.push_back(var);
                } else {
                        t--;
                        break;
                }

                // shift to next token
                t++;
        } while (1);

        // here we have a list of variables to assign
        // let's parse an expression
        RBasic::Value val = Grammar::Exp1(t, until);

        // restore context
        context = old_context;

        // assign values to all variables in list
        std::list<RBasic::Variable>::iterator i = vars.begin();

        // if no assignments need to be performed, just print results
        if (i == vars.end() && context) {
                std::cout << val;
        } else {
                for (; i != vars.end(); i++) {
                        RBasic::setVariable(*i, val);
                }
        }

        return val;
}

/**
 * @brief Parse variable from grammar
 *
 * @param token_iterator lst Pointer to current token in list
 * @return RBasic::Variable Resulting variable (or throw exception if
 *              there's no variable in stream, or it is function call
 */
RBasic::Variable Grammar::Variable(token_iterator& lst)
{
        Token elem = *lst;

        if (elem.type != TOKEN_ID) { // if it is not an identifier -> not a var
                throw NotAVariableException(); // nothing to parse
        } else {
                // check next token - for index expression or function call
                lst++;

                if (lst->type == TOKEN_OPBR) { // (
                        lst--;
                        throw IsFunctionCallException(); // it is a function call
                } else if (lst->type == TOKEN_OPIND) { // [
                        lst++;
                        // get index and return indexed expression
                        RBasic::Value index = Grammar::Expression(lst, Token(TOKEN_CLIND));

                        if (lst->type != TOKEN_CLIND) { // ]
                                // TODO: wait for end of expression, or die?
                        }

                        lst++; // skip ]
                        
                        return RBasic::getVariable(elem.str, index);
                }

                // get variable from context
                return RBasic::getVariable(elem.str);
        }
}

/**
 * @brief Parse ariphmetic expression
 */
RBasic::Value Grammar::Exp1(token_iterator &lst, const Token &until)
{
        std::list<Token> op_stack;
        std::list<RBasic::Value> val_stack;

        Token t = *lst;

        while (!(t == until)) {
                
                // check if token is a value
                // first - check if it is a variable or function call
                if (t.type == TOKEN_ID) {
                        RBasic::Value val;

                        // try to get variable
                        try {
                                val = Grammar::Variable(lst);
                                lst--; // return carriage
                        } catch (IsFunctionCallException &e) {
                                // this ID is a function call
                                val = Grammar::FunctionCall(lst);
                        } catch (...) {
                                // unknown type
                                // TODO: generate error
                        }

                        val_stack.push_back(val);
                } else if (t.type == TOKEN_NUMBER) {
                        val_stack.push_back(RBasic::Value(t.dbl));
                } else if (t.type == TOKEN_STRING) {
                        val_stack.push_back(RBasic::Value(t.str));
                } else if (t.type == TOKEN_TRUE) {
                        val_stack.push_back(RBasic::Value(RBasic::Elem(true, true)));
                } else if (t.type == TOKEN_FALSE) {
                        val_stack.push_back(RBasic::Value(RBasic::Elem(false, true)));
                } else if (t.type == TOKEN_NULL) {
                        val_stack.push_back(RBasic::Value());
                }

                // Check if current token is open bracket - need to get new expression
                else if (t.type == TOKEN_OPBR) {
                        lst++;
                        val_stack.push_back(Grammar::Expression(lst, Token(TOKEN_CLBR)));
                        // closing bracket is skipped already
                }

                // Check if current token is operator
                else if (t.isOperator()) {
                        if (op_stack.size() == 0) { // no operations in stack
                                // just push current operation
                                op_stack.push_back(t);
                        } else {
                                // check if current operation has less priority
                                if (op_stack.back().weight() > t.weight()) {
                                        // less priority - clean up the stack
                                        // TODO: error handling
                                        Grammar::Calculate(op_stack, val_stack);
                                }
                                // weight is OK, now push it in stack
                                op_stack.push_back(t);
                        }
                }

                lst++;
                t = *lst;
        }

        // clean up stack and so on
        // TODO: error handling
        Grammar::Calculate(op_stack, val_stack);

        // return top element of stack
        return val_stack.back();
}

bool Grammar::Calculate(std::list<Token> &op_stack, std::list<RBasic::Value> &val_stack)
{
        // get top operation from the stack and perform it
        while (!op_stack.empty()) {
                Token op = op_stack.back();
                op_stack.pop_back();

                if (val_stack.empty()) {
                        throw NoOperandException(op);
                }

                RBasic::Value exp2 = val_stack.back();
                val_stack.pop_back();

                if (val_stack.empty()) {
                        throw NoOperandException(op);
                }

                RBasic::Value exp1 = val_stack.back();
                val_stack.pop_back();

                val_stack.push_back(RBasic::Operation(op, exp1, exp2));
        }

        return true;
}

RBasic::Value Grammar::FunctionCall(token_iterator &lst)
{
        // if current token is not and ID - not a function call
        if (lst->type != TOKEN_ID) {
                throw NotAFunctionCallException();
        }

        lst++;

        // not an opening bracket - not a function call
        if (lst->type != TOKEN_OPBR) {

        }


        return RBasic::Value();
}
