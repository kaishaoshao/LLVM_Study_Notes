//===----------------------------------------------------------------------===//
//
// 本文件定义了Toy语言的抽象语法树（AST），设计注重简洁性而非效率。
// AST采用树形结构，使用std::unique_ptr<>管理子节点所有权。
//
//===----------------------------------------------------------------------===//

#ifndef __TOY_AST_H__
#define __TOY_AST_H__

#include "Lexer.h"


#include <vector>

namespace toy{


/// 变量类型（包含形状信息，用于张量）
struct VarType
{
    std::vector<int64_t> shape; // 形状维度数组，如[2,3]表示2x3矩阵
};

// 所有表达式节点的基类(抽象类)
class ExprAST{
public:
    /// AST 节点类型枚举
    enum ExprASTKind{
        Expr_VarDecl, // 变量声明节点
        Expr_Return,  // 返回节点
        Expr_Num,     // 数字字面量
        Expr_Literal, // 数组字面量
        Expr_Var,     // 变量引用
        Expr_Call,    // 函数调用
        Expr_Print,   // 打印节点
    };      
    
    /// 构造函数
    ExprAST(ExprASTKind kind, Location location)
         : kind(kind), location(location) {}


    /// 析构函数
    virtual ~ExprAST() = default; 
    
    /// 获取节点类型
    ExprASTKind getKind() const { return kind; }

    /// 获取源码位置信息
    const Location& getLocation() const { return location; }


private:
    const ExprASTKind kind;   // 节点类型标识
    const Location location;  // 源码位置（行序号）

}

/// 表达式列表类型 (用于代码块)
using ExprASTList = std::vector<std::unique_ptr<ExprAST>>;

/// 数字字面量节点（如 3.14）
class 





}


#endif // __TOY_AST_H__