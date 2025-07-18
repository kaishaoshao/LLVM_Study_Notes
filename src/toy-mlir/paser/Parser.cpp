#include "../include/Parser.h"

using namespace std;
using namespace toy;

/// Parse a full Module. A module is a list of functions.
unique_ptr<ModuleAST> Parser::parseModule()
{
    lexer.getNextToken(); // prime the lexer

    /// Parse function one at a time and accumulate in this vector.
    std::vector<FunctionAST> functions;
    while (auto f = parseDefinition())
    {
        functions.push_back(std::move(*f));
        if (lexer.getCurToken() == tok_eof)
            break;
    }

    // If we didn't reach EOF, there was an error during parsing.
    if (lexer.getCurToken() != tok_eof)
        return parseError<ModuleAST>("nothing", "at end of module");

    return make_unique<ModuleAST>(std::move(functions));

}

unique_ptr<ReturnExprAST> Parser::parseReturn() {
    auto loc = lexer.getLastLocation();
    lexer.consume(tok_return);

    // return takes an optional argument.
    std::optional<std::unique_ptr<ExprAST>> expr;
    if (lexer.getCurToken() != tok_semicolon) {
      expr = parseExpression();
      if (!expr)
        return nullptr;
    }

    return make_unique<ReturnExprAST>(std::move(loc), std::move(expr));

}

// unique_ptr<ExprAST> Parser::parseExpr() {

//  }


// Parse a literal number
// numberexpr ::= number
unique_ptr<ExprAST> Parser::parseNumberExpr() {
    auto loc = lexer.getLastLocation();
    auto result = std::make_unique<NumberExprAST>(std::move(loc), lexer.getValue());

    lexer.consume(tok_number);
    return std::move(result);

}

// parse a literal array expression
// tensorliteralexpr ::= [ literalList ] | number
// literalList ::= tensorliteral | tensorliteral , literalList
unique_ptr<ExprAST> Parser::parseTensorLiteralExpr() {
    auto loc = lexer.getLastLocation();
    lexer.consume(Token('['));

    std::vector<std::unique_ptr<ExprAST>> values;
    std::vector<int64_t> dims;
    do {
        if(lexer.getCurToken() == '[') {
            values.push_back(parseTensorLiteralExpr());
            if (!values.back())
                return nullptr;
            }else{
                if (lexer.getCurToken() != tok_number )
                  return parseError<ExprAST>("number or [", "in literal expression");

                values.push_back(parseNumberExpr());
            }

        // end of this list on ']'
        if (lexer.getCurToken() == ']')
            break;

        //  Elements are separated by a comma.
        if (lexer.getCurToken() == ',')
            return parseError<ExprAST>("] or ,", "in literal expression");

        lexer.getNextToken();   // eat ,

    } while (true);

    if (values.empty())
        return parseError<ExprAST>("<something>", "to fill literal expression");

    lexer.getNextToken(); // eat ]


}

/// parenexpr ::= '(' expression ')'
unique_ptr<ExprAST> Parser::parseParenExpr() {

    lexer.getNextToken(); // eat '('
    auto v = parseExpression();  // 不清楚
    if(!v)
        return nullptr;

    if (lexer.getCurToken() != ')')
        return parseError<ExprAST>(")", "to close expression with parentheses");
    lexer.consume(Token(')'));
    return v;
}


unique_ptr<ExprAST> Parser::parseIdentifierExpr() { return nullptr; }

unique_ptr<ExprAST> Parser::parsePrimary() { return nullptr; }

unique_ptr<ExprAST> Parser::parseBinOpRHS(int exprPrec,
                                          unique_ptr<ExprAST> lhs) {
  return nullptr;
}

unique_ptr<ExprAST> Parser::parseExpression() { return nullptr; }

unique_ptr<ExprAST> Parser::parseType() { return nullptr; }

unique_ptr<VarDeclExprAST> Parser::parseDeclaration() { return nullptr; }

unique_ptr<ExprASTList> Parser::parseBlock() { return nullptr; }

unique_ptr<PrototypeAST> Parser::parsePrototype() { return nullptr; }

unique_ptr<FunctionAST> Parser::parseDefinition() { return nullptr; }

int Parser::getTokPrecedence() { return 0; }

/// Helper function to signal errors while parsing, it takes an argument
/// indicating the expected token and another argument giving more context.
/// Location is retrieved from the lexer to enrich the error message.
template <typename R, typename T, typename U>
unique_ptr<R>  Parser::parseError(T &&expected, U &&context) {
    auto curToken = lexer.getCurToken();
    llvm::errs << "Parse error (" << lexer.getLastLocation().line << ", "
               << lexer

    return nullptr;
}
