// 抽象语法树的作用在于抓住程序的脉络，方便代码生成对程序的解读
// 递归下降解析和运算符优先级解析

#include "../lexer/lexer.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>


class ExprAST{
public:
    virtual ~ExprAST() = default; // 没有动态分配内存或其他需要特别处理的资源，使用 = default 是合适的。
};

// 用于表示像1.0这样的数值字面量
// NumberExpeAST可以访问ExprAST中的所有公开成员
class NumberExprAST:public ExprAST
{
    double Val;
public:
    // 接受参数Val并将其复制给类成员Val
    NumberExprAST(double Val): Val(Val) {}  
};

// 用于引用变量的表达是类，例如变量‘a’
class VariableExperAST: public ExprAST{
    std::string Name;
public:
    VariableExperAST(const std::string &Name): Name(Name){}
};

// 二元操作符的表达式
class BinaryExprAST: public ExprAST{
    char Op;                          // 用来存储'+' '-'
    std::unique_ptr<ExprAST> LHS,RHS; // 左右操作数
public:
    BinaryExprAST(char Op,std::unique_ptr<ExprAST> LHS,
                    std::unique_ptr<ExprAST> RHS)
                    :Op(Op),LHS(std::move(LHS)),RHS(std::move(RHS)) {}
};

// 函数调用表达式
class CallExprAST: public ExprAST{
    std::string Callee;                         // 用于储存呗调用函数的名称
    std::vector<std::unique_ptr<ExprAST>> Args; // 存储函数被调用时的传递的参数列表
public:
    CallExprAST(const std::string &Callee,      
    std::vector<std::unique_ptr<ExprAST>> Args)
    : Callee(Callee),Args(std::move(Args)) {}
};
// 其特点是无须关注语法便科直接抓住语言本身的特性，这里并没有涉及二元运算符的优先级和词法结构等问题
// 定义完这几种表达式节点，就能描述Kaleidoscope语言中的几个最基本的结构，但是还不算图灵完备，
// 还需要描述函数接口和函数本身

// 代表函数原型 用于捕获（存储）函数的名称和参数名称，从而间接地表示函数所接受的参数数量
// 定义一个函数的基本信息，包括函数名称和参数列表，通过参数列表可以知道该函数接受多少个参数。
class PrototypeAST
{
    std::string Name;
    std::vector<std::string> Args;

public:
    PrototypeAST(const std::string &Name,
                    std::vector<std::string> Args)
        :Name(Name),Args(std::move(Args)) {}

    // 获取函数名称 不需要复制这个名称
    const std::string &getName() const {return Name;}

};


// 函数定义本身
class FunctionAST
{
    std::unique_ptr<PrototypeAST> Proto;  // 表示函数原型
    std::unique_ptr<ExprAST> Body;        // 表示函数体表达式

public:
    FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                    std::unique_ptr<ExprAST> Body)
        :Proto(std::move(Proto)),Body(std::move(Body)) {}
};
// 在Kaleidoscope中，函数的类型化只需要它们的参数，由于所有值都是双精度浮点数，
// 因此type不需要存储在任何地方。在更多激进而现实的语言里，“ExprAST”可能会具有type字段


