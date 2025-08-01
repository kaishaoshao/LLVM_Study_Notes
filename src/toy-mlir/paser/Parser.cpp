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

    // Hold
    std::vector<std::unique_ptr<ExprAST>> values;
    std::vector<int64_t> dims;
    do {
        //We can have either another nested array or a number literal.
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
        if (lexer.getCurToken() != ',')
            return parseError<ExprAST>("] or ,", "in literal expression");

        lexer.getNextToken();   // eat ,

    } while (true);

    if (values.empty())
        return parseError<ExprAST>("<something>", "to fill literal expression");

    lexer.getNextToken(); // eat ]

    /// Fill in the dimensions now. First the current nesting level:
    dims.push_back(values.size());

    /// 如果存在任何嵌套数组，则处理所有这些数组，并确保其维度一致
    if (llvm::any_of(values, [](std::unique_ptr<ExprAST> &expr)
                     { return llvm::isa<LiteralExprAST>(expr.get());
      })) {
      auto *firstLiteral = llvm::dyn_cast<LiteralExprAST>(values.front().get());
      if (!firstLiteral)
        return parseError<ExprAST>("uniform well-nested dimensions",
                                    "inside literal expression");
      // Append the nested dimensions to the current level
      auto firstDims = firstLiteral->getDims();
      dims.insert(dims.end(), firstDims.begin(), firstDims.end());
      // Sanity check that shape is uniform across all elements of the list.
      for (auto &expr : values) {
        auto *exprLiteral = llvm::cast<LiteralExprAST>(expr.get());
        if (!exprLiteral)
          return parseError<ExprAST>("uniform well-nested dimensions",
                                     "inside literal expression");
        if (exprLiteral->getDims() != firstDims)
          return parseError<ExprAST>("uniform well-nested dimensions",
                                     "inside literal expression");
      }
    }
    return std::make_unique<LiteralExprAST>(std::move(loc), std::move(values),
                                            std::move(dims));
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

/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression ')'
unique_ptr<ExprAST> Parser::parseIdentifierExpr() {
  std::string name(lexer.getId());

  auto loc = lexer.getLastLocation();
  lexer.getNextToken(); // eat identifier.
                        //
  if (lexer.getCurToken() != '(') // Simple variable ref.
    return std::make_unique<VariableExprAST>(std::move(loc), name);

  // This is a function call.
  lexer.consume(Token('('));
  std::vector<std::unique_ptr<ExprAST>> args;
  if (lexer.getCurToken() != ')') {
    while (true) {
      if (auto arg = parseExpression())
        args.push_back(std::move(arg));
      else
        return nullptr;

      if (lexer.getCurToken() == ')')
        break;

      if (lexer.getCurToken() != ',')
        return parseError<ExprAST>(", or )", "in argument list");
      lexer.getNextToken();
    }
  }
  lexer.consume(Token(')'));

  // It can be a builtin call to print
  if (name == "print") {
    if (args.size() != 1)
      return parseError<ExprAST>("<single arg>", "as argument to print()");

    return std::make_unique<PrintExprAST>(std::move(loc), std::move(args[0]));
  }

  // Call to a user-defined function
  return std::make_unique<CallExprAST>(std::move(loc), name, std::move(args));
}

/// primary
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr
///   ::= tensorliteral
unique_ptr<ExprAST> Parser::parsePrimary() {
  switch (lexer.getCurToken()) {
  default:
    llvm::errs() << "unknown token '" << lexer.getCurToken()
                 << "' when expecting an expression\n";
    return nullptr;
  case tok_identifier:
    return parseIdentifierExpr();
  case tok_number:
    return parseNumberExpr();
  case '(':
    return parseParenExpr();
  case '[':
    return parseTensorLiteralExpr();
  case ';':
    return nullptr;
  case '}':
    return nullptr;
  }
}

/// Recursively parse the right hand side of a binary expression, the ExprPrec
/// argument indicates the precedence of the current binary operator.
///
/// binoprhs ::= ('+' primary)*
unique_ptr<ExprAST> Parser::parseBinOpRHS(int exprPrec,
                                          unique_ptr<ExprAST> lhs) {
  // If this is a binop, find its precedence.
  while (true) {
    int tokPrec = getTokPrecedence();

    // If this is a binop that binds at least as tightly as the current binop,
    // consume it, otherwise we are done.
    if (tokPrec < exprPrec)
      return lhs;

    // Okay, we know this is a binop.
    int binOp = lexer.getCurToken();
    lexer.consume(Token(binOp));
    auto loc = lexer.getLastLocation();

    // Parse the primary expression after the binary operator.
    auto rhs = parsePrimary();
    if (!rhs)
      return parseError<ExprAST>("expression", "to complete binary operator");

    // If BinOp binds less tightly with rhs than the operator after rhs, let
    // the pending operator take rhs as its lhs.
    int nextPrec = getTokPrecedence();
    if (tokPrec < nextPrec) {
      rhs = parseBinOpRHS(tokPrec + 1, std::move(rhs));
      if (!rhs)
        return nullptr;
    }

    // Merge lhs/RHS.
    lhs = std::make_unique<BinaryExprAST>(std::move(loc), binOp, std::move(lhs),
                                          std::move(rhs));
  }
}

/// expression::= primary binop rhs
unique_ptr<ExprAST> Parser::parseExpression() {
  auto lhs = parsePrimary();
  if (!lhs)
    return nullptr;

  return parseBinOpRHS(0, std::move(lhs));
}

/// type ::= < shape_list >
/// shape_list ::= num | num , shape_list
unique_ptr<VarType> Parser::parseType() {
  if (lexer.getCurToken() != '<')
    return parseError<VarType>("<", "to begin type");
  lexer.getNextToken(); // eat <

  auto type = std::make_unique<VarType>();

  while (lexer.getCurToken() == tok_number) {
    type->shape.push_back(lexer.getValue());
    lexer.getNextToken();
    if (lexer.getCurToken() == ',')
      lexer.getNextToken();
  }

  if (lexer.getCurToken() != '>')
    return parseError<VarType>(">", "to end type");
  lexer.getNextToken(); // eat >
  return type;
}

/// Parse a variable declaration, it starts with a `var` keyword followed by
/// and identifier and an optional type (shape specification) before the
/// initializer.
/// decl ::= var identifier [ type ] = expr
unique_ptr<VarDeclExprAST> Parser::parseDeclaration() {
  if (lexer.getCurToken() != tok_var)
    return parseError<VarDeclExprAST>("var", "to begin declaration");
  auto loc = lexer.getLastLocation();
  lexer.getNextToken(); // eat var

  if (lexer.getCurToken() != tok_identifier)
    return parseError<VarDeclExprAST>("identified", "after 'var' declaration");
  std::string id(lexer.getId());
  lexer.getNextToken(); // eat id

  std::unique_ptr<VarType> type; // Type is optional, it can be inferred
  if (lexer.getCurToken() == '<') {
    type = parseType();
    if (!type)
      return nullptr;
  }

  if (!type)
    type = std::make_unique<VarType>();
  lexer.consume(Token('='));
  auto expr = parseExpression();
  return std::make_unique<VarDeclExprAST>(std::move(loc), std::move(id),
                                          std::move(*type), std::move(expr));
}

/// Parse a block: a list of expression separated by semicolons and wrapped in
/// curly braces.
///
/// block ::= { expression_list }
/// expression_list ::= block_expr ; expression_list
/// block_expr ::= decl | "return" | expr
unique_ptr<ExprASTList> Parser::parseBlock() {
  if (lexer.getCurToken() != '{')
    return parseError<ExprASTList>("{", "to begin block");
  lexer.consume(Token('{'));

  auto exprList = std::make_unique<ExprASTList>();

  // Ignore empty expressions: swallow sequences of semicolons.
  while (lexer.getCurToken() == ';')
    lexer.consume(Token(';'));

  while (lexer.getCurToken() != '}' && lexer.getCurToken() != tok_eof) {
    if (lexer.getCurToken() == tok_var) {
      // Variable declaration
      auto varDecl = parseDeclaration();
      if (!varDecl)
        return nullptr;
      exprList->push_back(std::move(varDecl));
    } else if (lexer.getCurToken() == tok_return) {
      // Return statement
      auto ret = parseReturn();
      if (!ret)
        return nullptr;
      exprList->push_back(std::move(ret));
    } else {
      // General expression
      auto expr = parseExpression();
      if (!expr)
        return nullptr;
      exprList->push_back(std::move(expr));
    }
    // Ensure that elements are separated by a semicolon.
    if (lexer.getCurToken() != ';')
      return parseError<ExprASTList>(";", "after expression");

    // Ignore empty expressions: swallow sequences of semicolons.
    while (lexer.getCurToken() == ';')
      lexer.consume(Token(';'));
  }

  if (lexer.getCurToken() != '}')
    return parseError<ExprASTList>("}", "to close block");

  lexer.consume(Token('}'));
  return exprList;
}

/// prototype ::= def id '(' decl_list ')'
/// decl_list ::= identifier | identifier, decl_list
unique_ptr<PrototypeAST> Parser::parsePrototype() {
  auto loc = lexer.getLastLocation();

  if (lexer.getCurToken() != tok_def)
    return parseError<PrototypeAST>("def", "in prototype");
  lexer.consume(tok_def);

  if (lexer.getCurToken() != tok_identifier)
    return parseError<PrototypeAST>("function name", "in prototype");

  std::string fnName(lexer.getId());
  lexer.consume(tok_identifier);

  if (lexer.getCurToken() != '(')
    return parseError<PrototypeAST>("(", "in prototype");
  lexer.consume(Token('('));

  std::vector<std::unique_ptr<VariableExprAST>> args;
  if (lexer.getCurToken() != ')') {
    do {
      std::string name(lexer.getId());
      auto loc = lexer.getLastLocation();
      lexer.consume(tok_identifier);
      auto decl = std::make_unique<VariableExprAST>(std::move(loc), name);
      args.push_back(std::move(decl));
      if (lexer.getCurToken() != ',')
        break;
      lexer.consume(Token(','));
      if (lexer.getCurToken() != tok_identifier)
        return parseError<PrototypeAST>("identifier",
                                        "after ',' in function parameter list");
    } while (true);
  }
  if (lexer.getCurToken() != ')')
    return parseError<PrototypeAST>(")", "to end function prototype");

  // success.
  lexer.consume(Token(')'));
  return std::make_unique<PrototypeAST>(std::move(loc), fnName,
                                        std::move(args));
}

/// Parse a function definition, we expect a prototype initiated with the
/// `def` keyword, followed by a block containing a list of expressions.
///
/// definition ::= prototype block
unique_ptr<FunctionAST> Parser::parseDefinition() {
  auto proto = parsePrototype();
  if (!proto)
    return nullptr;

  if (auto block = parseBlock())
    return std::make_unique<FunctionAST>(std::move(proto), std::move(block));
  return nullptr;
}

// 获取当前词法标记（token）的运算符优先级
int Parser::getTokPrecedence() {
    if (!isascii(lexer.getCurToken()))
      return -1;

    // 1 is lowest precedence.
    switch (static_cast<char>(lexer.getCurToken()))
    {
    case '-':
        return 20;
    case '+':
        return 20;
    case '*':
        return 40;
    default:
        return -1;
    }

    return 0;
}

/// Helper function to signal errors while parsing, it takes an argument
/// indicating the expected token and another argument giving more context.
/// Location is retrieved from the lexer to enrich the error message.
template <typename R, typename T, typename U>
unique_ptr<R>  Parser::parseError(T &&expected, U &&context) {
    auto curToken = lexer.getCurToken();
    llvm::errs() << "Parse error (" << lexer.getLastLocation().line << ", "
               << lexer.getLastLocation().col << "): expected '" << expected
               << "' " << context << " but has Token " << curToken;
    if(isprint(curToken))
        llvm::errs() << " '" << (char)curToken << " '";
    llvm::errs() << "\n";
    return nullptr;
}
