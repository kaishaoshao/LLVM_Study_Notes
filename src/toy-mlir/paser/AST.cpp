//===----------------------------------------------------------------------===//
//
// 该文件实现了Toy语言的抽象语法树(AST)转储功能，用于调试和可视化AST结构。
//
//===----------------------------------------------------------------------===//

#include "../include/AST.h"

#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/Twine.h>

using namespace toy;

namespace {

// 缩进管理辅助类 (RAII模式)
struct Indent{
    Indent(int &level) : level(level) { ++level; } // 构造时增加缩进层级
    ~Indent() { --level; }                        // 析构时减少缩进层级
    int &level;  

};

/// AST转储器实现类，负责遍历AST并格式化输出节点信息
class ASTDumper {
public:
void dump(ModuleAST *node); // 模块入口点

private:
    // 各类型节点的转储方法
    void dump(const VarType &type);
    void dump(VarDeclExprAST *VarDecl);
    void dump(ExprAST *expr);
    void dump(ExprASTList *exprList);
    void dump(NumberExprAST *num);
    void dump(LiteralExprAST *node);
    void dump(VariableExprAST *node);
    void dump(ReturnExprAST *node);
    void dump(BinaryExprAST *node);
    void dump(CallExprAST *node);
    void dump(PrintExprAST *node);
    void dump(PrototypeAST *node);
    void dump(FunctionAST *node);

    // 缩进输出辅助方法
    void indent() {
        for (int  i = 0; i < curIndent; i++)
            llvm::errs() << " ";  // 每个缩进级别输出两个空格
    }

    int curIndent = 0 ; // 当前缩进级别
};  

}  // 匿名命名空间


/// 生成节点位置信息的格式化字符串
template <typename T>
static std::string loc(T *node) {
    const auto &loc = node->loc();
    return (llvm::Twine("@") + *loc.file + ":"+ llvm::Twine(loc.line) + ":" +
            llvm::Twine(loc.col)).str();
}


// 缩进宏： 进入作用域增加缩进，离开时自动恢复
#define INDENT()                                             \
        Indent level_(curIndent);     /* RAII对象管理缩进 */   \  
        index();                      /* 输出当前缩进空格*/


/// 表达式动态分发处理：根据具体子类型调用对应的转储方法
void ASTDumper::dump(ExprAST *expr) {

}


void ASTDumper::dump(const VarType &type){

}

void ASTDumper::dump(VarDeclExprAST *VarDecl){

}

void ASTDumper::dump(ExprASTList *exprList){

}

void ASTDumper::dump(NumberExprAST *num){

}

void ASTDumper::dump(LiteralExprAST *node) {

}

void ASTDumper::dump(VariableExprAST *node){

}

void ASTDumper::dump(ReturnExprAST *node){

}

void ASTDumper::dump(BinaryExprAST *node){

}

void ASTDumper::dump(CallExprAST *node){

}

void ASTDumper::dump(PrintExprAST *node){

}

void ASTDumper::dump(PrototypeAST *node){

}

void ASTDumper::dump(FunctionAST *node){
    
}