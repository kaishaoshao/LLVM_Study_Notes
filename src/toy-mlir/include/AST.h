//===----------------------------------------------------------------------===//
//
// 本文件定义了Toy语言的抽象语法树（AST），设计注重简洁性而非效率。
// AST采用树形结构，使用std::unique_ptr<>管理子节点所有权。
//
//===----------------------------------------------------------------------===//

#ifndef __TOY_AST_H__
#define __TOY_AST_H__

#include "Lexer.h"

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/Casting.h>

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

};

/// 表达式列表类型 (用于代码块)
using ExprASTList = std::vector<std::unique_ptr<ExprAST>>;

class NumberExprAST : public ExprAST {
private:
    double val;  // 存储数值

public:
    NumberExprAST(Location loc, double val) : 
                ExprAST(Expr_Num, std::move(loc), val(val)) {}
    /// 获取数值
    double getValue() { return val; }
    /// llvm风格类型检查（用于安全向下转型）
    static bool classof(const ExprAST *c) { 
        return c->getKind() == Expr_Num;
    }

};

/// 数组字面量节点（ 数组字面量节点（如[[1,2],[3,4]]）
class  LiteralExprAST: public ExprAST {
private:
    std::vector<std::unique_ptr<ExprAST>> values; // 嵌套元素
    std::vector<int64_t> dims;                    // 维度信息

public:
    LiteralExprAST(Location loc, std::vector<std::unique_ptr<ExprAST>> values,
        std::vector<int64_t> dims) : ExprAST(Expr_Literal, std::move(loc)),
                                    values(std::move(values)), dims(std::move(dims)) {}
    
    /// 获取所有元素（包括嵌套数组）
    llvm::ArrayRef<std::unique_ptr<ExprAST>> getValues() {
        return values;
    }

    /// 获取维度信息（如[2,3]表示2x3矩阵）
    llvm::ArrayRef<int64_t> getDims() {return dims;}

    //  llvm风格类型检查 同上
    static bool classof(const ExprAST *c) {
        return c->getKind() == Expr_Literal;
    }

};

/// 变量引用节点 (如访问变量a)
class VariableExprAST : public ExprAST {
private:
    std::string name; // 变量名

public:
    VariableExprAST(Location loc, llvm::StringRef name) 
        : ExprAST(Expr_Var, std::move(loc)), name(name) {}

    /// 获取变量名
    llvm::StringRef getName() { return name; }

    static bool classof(const ExprAST *c) {
        return c->getKind() == Expr_Var;
    }

};


/// 变量声明节点（如 var a = 5;）
class VarDeclExprAST: public ExprAST {
private:
    std::string name;  // 变量
    VarType type;      // 变量类（含形状）
    std::unique_ptr<ExprAST> initVal; // 初始化表达式

public:
    VarDeclExprAST(Location loc, llvm::StringRef name, VarType type, 
        std::unique_ptr<ExprAST> initVal) : ExprAST(Expr_VarDecl, std::move(loc)),
        name(name), type(std::move(type)), initVal(std::move(initVal)) {}

    /// 获取变量名
    llvm::StringRef getName() { return name; }

    /// 获取初始化表达式
    ExprAST *getInitVal() { return initVal.get(); }

    /// 获取变量类型信息
    const VarType &getType() { return type; }

    static bool classof(const ExprAST *c) {
        return c->getKind() == Expr_VarDecl;
    }

};

/// 返回语句节点（如return 42;）
class ReturnExprAST : public ExprAST {

};


/// 二元运算节点 (如a+b)
class BinaryExprAST : public ExprAST {

};


/// 函数调用节点（如multiply(a, b)）
class CallExprAST : public ExprAST {

};

/// 打印语句节点（如print(c);）
class PrintExprAST: public ExprAST {

};


/// 函数原型节点 (定义函数签名)
class PrototypeAST {

};

/// 函数定义节点（原型+函数体）
class FunctionAST {

};

/// 模块节点（包含多个函数定义）
class ModuleAST {

};

/// 模块AST调试输出函数声明
void dump(ModuleAST &);

} // namespace toy


#endif // __TOY_AST_H__