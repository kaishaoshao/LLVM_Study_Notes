
#include "parser.h"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <memory>
#include <map>
#include <utility>
#include <vector>

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


// 

static std::unique_ptr<ExprAST> ParseIdentifierExpr(){
    std::string IdName = IdentifierStr;
    getNextToken(); // eat identifier

    if(CurTok != '(')
        return std::make_unique<VariableExperAST> (IdName);    

    //
    getNextToken(); // eat (
    std::vector<std::unique_ptr<ExprAST>> Args;
    if (CurTok != ')')
    {
        while (true) {
            if (auto Arg = ParseExpression()) {
                Args.push_back(std::move(Arg));
            }
            else { 
                return nullptr;
            }
            if(CurTok == ')')
                break;
            if(CurTok == ',')
                return LogError("Expected ')' or ',' in argument list");
            getNextToken();
        }
    }
    getNextToken();  // eat the ')'

    return std::make_unique<CallExprAST>(IdName,std::move(Args));
}

// 

static std::unique_ptr<ExprAST> ParsePrimary(){
    switch (CurTok) {
        default:
            return LogError("Unknown token when expecting an expression");
        case tok_identifier:
            return ParseIdentifierExpr();
        case tok_number:
            return ParseNumberExpr();
        case '(':
            return ParseParenExpr();
    }
}


// 二进制表达式解析
static int GetTokPrecedence(){
    if (!isascii(CurTok))   // ASCII
        return -1;
    // 确保当前运算符是一个已声明的二元运算符
    int TokPrec = BinopPrecedence[CurTok];
    if(TokPrec <= 0)
        return -1;
    return TokPrec;
}
// 对于Kaleidoscope的基本形式，只支持4个二进制操作符（可以自己拓展）
// 该函数返回为当前令牌，如果令牌不是二进制运算符，则为-1
// 解析二进制文件表达式
// 运算符优先级的基本思想是将具有歧义的二元运算符表达式分解成若干部分，
// 例如，对于表达式“a+b(c+d)*e*f+g”它会解析为前导式a,[+,b],[+,(c+d)]
// [*,e][*,f]和[+,g]


static std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec,
                                    std::unique_ptr<ExprAST> LHS){

    while (true){
        int TokPrec = GetTokPrecedence();
    if (TokPrec < ExprPrec) 
        return LHS;
    int BinOp = CurTok;
    getNextToken();

    auto RHS = ParsePrimary();
    if(!RHS)
        return nullptr;

    int NextPrec = GetTokPrecedence();
    if(TokPrec < NextPrec){
        RHS = ParseBinOpRHS(TokPrec+1, std::move(RHS));
        if(!RHS)
            return nullptr;
        }

        // Merge LHS/RHS
        LHS = std::make_unique<BinaryExprAST>(BinOp,std::move(LHS),std::move(RHS));
    }
}

// 处理变量引用和函数调用
static std::unique_ptr<ExprAST> ParseExpression(){
    auto LHS = ParsePrimary();
    if(!LHS)
        return nullptr;
    return ParseBinOpRHS(0, std::move(LHS));
}

// prototype
//   ::= id '(' id* ')' 
static std::unique_ptr<PrototypeAST> ParsePrototype(){
    if(CurTok != tok_identifier)
        return LogErrorP("Expected function name in prototype");
    std::string FnName = IdentifierStr;
    getNextToken();
    if(CurTok != '(')
        return LogErrorP("Expected '(' in prototype");
    std::vector<std::string> ArgNames;
    while (getNextToken() == tok_identifier) {
        ArgNames.push_back(IdentifierStr);
    }
    if(CurTok != ')')
        return LogErrorP("Expected ')' in prototype");

    // success
    getNextToken(); // eat ')'
    return std::make_unique<PrototypeAST>(FnName,std::move(ArgNames));
}

//  definition ::= 'def' prototype expression
static std::unique_ptr<FunctionAST>  ParseDefinition(){
    getNextToken();
    auto Proto = ParsePrototype();
    if(!Proto)
        return nullptr;
    if(auto E = ParseExpression())
        return std::make_unique<FunctionAST>(std::move(Proto),std::move(E));
    return nullptr;
}

// toplevelexpr 
static std::unique_ptr<FunctionAST> ParseTopLevelExpr(){
    if(auto E = ParseExpression()){
        // Make an anonymous proto 
        auto Proto = std::make_unique<PrototypeAST>("__anon_expr",
                                                    std::vector<std::string>());
        return std::make_unique<FunctionAST>(std::move(Proto),std::move(E));
    }
    return nullptr ;
} 

// external ::= 'extern' prototype
static std::unique_ptr<PrototypeAST> ParseExtern(){
    getNextToken();
    return ParsePrototype();
}

static void HandleDefinition(){
    if(ParseDefinition()){
        fprintf(stderr, "Parsed a function definition.\n");
    }else{
        // skip token for error recovery
        getNextToken(); 
    }
}

static void HandleExtern()
{
    if(ParseExtern()){
        fprintf(stderr, "Parsed an extern\n");
    }
    else {
        // skip token for error recovery
        getNextToken();
    }
}

static void HandleTopLevelExpression(){
    // 
    if(ParseTopLevelExpr()){
        fprintf(stderr, "Parsed a top-level expr\n");
    }
    else {
        //
        getNextToken();
    }
}













