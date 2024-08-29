
#include "parser.h"
#include <memory>


// 词法解析器
// 上面构建了一个AST，我们需要定义解析器代码来构建
// 实现解析类似于“x+y”的公式，将这三个字符放入AST中
auto LHS = std::make_unique<VariableExperAST>("x");
auto RHS = std::make_unique<VariableExperAST>("y");
auto Result = std::make_unique<BinaryExprAST>('+',std::move(LHS),std::move(RHS));


// 基本表达式解析
static std::unique_ptr<ExprAST> ParseNumberExpr(){
    auto Result = std::make_unique<NumberExprAST>(NumVal);
    getNextToken();             // 获取下一个token(令牌) 
    return std::move(Result);   // 将result智能指针的所有权给调用者
}

// 解析括号表达式  parenexpr ::= '(' expression ')'
// It shows how we use the LogError routines. When called,
// this function expects that the current token is a ‘(’ token,
// but after parsing the subexpression, it is possible that there is no ‘)’ waiting. For example,
// if the user types in “(4 x” instead of “(4)”, the parser should emit an error.
static std::unique_ptr<ExprAST> ParseParenExpr(){
    getNextToken(); // 消灭'('
    auto V = ParseExpression();   // 还没有实现 用于解析表达式
    if (!V) {
        return nullptr;
    }
    if(CurTok != ')'){
        return LogError("expected ')'");
    }
    getNextToken(); //eat ')'
    return V; 
}
 
// 处理变量引用和函数调用
static std::unique_ptr<ExprAST> ParseExpression(){
    std::string IdName = IdentifierStr;  // 用来存储当前的标识符名称
    getNextToken();                      // eat identifier.

    if (CurTok != 'c') {
        return std::make_unique<VariableExperAST>(IdName);
    }


    
}













