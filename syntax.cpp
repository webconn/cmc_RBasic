#include "include/syntax.h"

//static bool cont = false;
static Token until = Token();

/**
 * @brief Parse token stream using syntax analyzer
 *
 * Take bytestream, create token list and parse it
 *
 * @param token_iterator& in Input bytestream
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
        t.printStart();

        while (!t.eof()) {

                try {
                        Grammar::Expression(t);
                } catch (RBasic::TypeException &e) {
                        std::cerr << "2 Wrong types of operation" << std::endl;
                } catch (RBasic::WrongArgumentsException &e) {
                        std::cerr << "3 Wrong arguments for function" << std::endl;
                } catch (Grammar::EmptyExpressionException &e) {

                }

                while (!t.eof() && (t->type == TOKEN_END || t->type == TOKEN_ENDS)) {
                        if (t->type == TOKEN_END) {
                                t.printStart();
                        }

                        t++;
                }
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

        // if last var in list will be without assignment,
        // we'll need to push it into calculation stack via
        // special Exp1 function call
        bool addToStack = false;
        RBasic::Value add_val;

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
                        // it was a first part of expression
                        addToStack = true;
                        add_val = var.getValue();
                        break;
                }

                // shift to next token
                t++;
        } while (1);

        // here we have a list of variables to assign
        // let's parse an expression
        RBasic::Value val;
        try {
                val = Grammar::Exp1(t, until, true, addToStack, add_val);
        } catch (...) {
                context = old_context;
                throw;
        }

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
RBasic::Value Grammar::Exp1(token_iterator &lst, const Token &until, bool fail, bool add_val, const RBasic::Value &val)
{
        std::list<Token> op_stack;
        std::list<RBasic::Value> val_stack;
        std::vector<bool> read_more_stack;

        if (add_val) {
                val_stack.push_back(val);
        }

        while (1) {

                if (lst->isValue()) {
                        if (read_more_stack.size() > 0) {
                                read_more_stack.pop_back();
                        }
                }

                // check if token is a value
                // first - check if it is a variable or function call
                if (lst->type == TOKEN_ID) {
                        RBasic::Value val;

                        // try to get variable
                        try {
                                val = Grammar::Variable(lst);
                                lst--; // return carriage
                        } catch (IsFunctionCallException &e) {
                                // this ID is a function call
                                val = Grammar::FunctionCall(lst);
                                lst--; // return carriage
                        } catch (...) {
                                // unknown type
                                // TODO: generate error
                        }

                        val_stack.push_back(val);
                } else if (lst->type == TOKEN_NUMBER) {
                        val_stack.push_back(RBasic::Value(lst->dbl));
                } else if (lst->type == TOKEN_STRING) {
                        val_stack.push_back(RBasic::Value(lst->str));
                } else if (lst->type == TOKEN_TRUE) {
                        val_stack.push_back(RBasic::Value(RBasic::Elem(true, true)));
                } else if (lst->type == TOKEN_FALSE) {
                        val_stack.push_back(RBasic::Value(RBasic::Elem(false, true)));
                } else if (lst->type == TOKEN_NULL) {
                        val_stack.push_back(RBasic::Value());
                }

                // Check if current token is open bracket - need to get new expression
                else if (lst->type == TOKEN_OPBR) {
                        lst++;
                        val_stack.push_back(Grammar::Expression(lst, Token(TOKEN_CLBR)));
                        // closing bracket is skipped already
                }

                // Check if current token is operator
                else if (lst->isOperator()) {
                        read_more_stack.push_back(true);

                        if (op_stack.size() == 0) { // no operations in stack
                                // just push current operation
                                op_stack.push_back(*lst);
                        } else {
                                // check if current operation has less priority
                                if (op_stack.back().weight() > lst->weight()) {
                                        // less priority - clean up the stack
                                        // TODO: error handling
                                        Grammar::Calculate(op_stack, val_stack);
                                }
                                // weight is OK, now push it in stack
                                op_stack.push_back(*lst);
                        }
                } 
                
                // Unexpected token
                else { 
                        if (lst->type == TOKEN_END) {
                                if (read_more_stack.size() != 0) {
                                        while (lst->type == TOKEN_END) {
                                                lst.printMore();
                                                lst++;
                                        }
                                        continue;
                                }
                        }

                        if (*lst == until) {
                                break;
                        }

                        if (lst.end() || lst->type == TOKEN_EOF)
                                break;

                        if (fail) { // if we need to fail
                                throw UnexpectedTokenException();
                        } else {
                                break;
                        }
                }

                lst++;
        }

        // clean up stack and so on
        // TODO: error handling
        Grammar::Calculate(op_stack, val_stack);

        if (val_stack.size() == 0) { // empty stack means empty expression
                throw EmptyExpressionException();
        }

        if (val_stack.size() != 1) { // stack must contain only 1 value
                throw UnexpectedTokenException();
        }

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

        Token id = *lst;

        lst++;

        // not an opening bracket - not a function call
        if (lst->type != TOKEN_OPBR) {
                throw NotAFunctionCallException();
                // we are unable to fix token iterator cause only 1 step 
                // backward is available
        }

        lst++; // skip bracket

        Token oldUntil = lst.readUntil(Token(TOKEN_CLBR));
        RBasic::ArgList args = Grammar::ArgList(lst);     
        lst.readUntil(oldUntil);

        // last bracket is skipped
        lst++;
        
        return RBasic::FunctionCall(id.str, args);
}

RBasic::ArgList Grammar::ArgList(token_iterator &it)
{
        RBasic::ArgList lst;

        while (!it.end()) {
                lst.push_back(RBasic::Argument(Grammar::Expression(it, Token(TOKEN_COMMA))));

                if (it->type == TOKEN_COMMA) {
                        it++;
                } else if (it->type == TOKEN_CLBR) {
                        break;
                }
        }

        return lst;
}
