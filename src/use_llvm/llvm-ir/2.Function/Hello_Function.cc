
// 创建了一个模块(Module)，接下来我们就在模块中加入一个函数。其实，模块也好，函数也好，都是LLVM IR代码。
// 如果要创建一个函数，我们还需要用到其它工具，比如llvm::Function、llvm::FunctionType等。
// 函数也是有类型的，而llvm::FunctionType就是用来创建函数类型的。创建函数之前，需要先创建其类型，
// 然后才能用llvm::Function来创建函数。

// 1.llvm/IR/Function.h：
// 包含了 Function 类的定义，该类代表 LLVM IR 中的一个函数。函数包含了一系列的指令（如基本块、指令和操作数）。
// 2.llvm/IR/FunctionType.h：
// 提供了 verifyFunction 和 verifyModule 函数，用于检查 LLVM IR 的一致性和正确性。这有助于在进一步处理前确保 IR 是有效的。
// 3.llvm/IR/LLVMContext.h：
// 定义了 LLVMContext 类，它是所有 LLVM IR 对象的上下文容器。IR 对象如类型、常量、模块等都需要一个 LLVMContext 实例来唯一标识它们。
// 4.llvm/IR/Module.h：
// 包含了 Module 类的定义，该类代表一个 LLVM IR 模块，可以包含多个函数、全局变量、别名和数据。

#include "llvm/IR/Function.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

int main(int argc, char **argv) {
    // 创建LLVM上下文，用于存储LLVM实体的共享全局状态
    llvm::LLVMContext context;

    // 创建一个新的模块，这是LLVM IR代码的容器
    llvm::Module* module = new llvm::Module("my_module", context); 

    // 创建一个返回空类型的函数类型
    // 获取空类型（voidType），用于定义函数返回类型。
    // 创建一个函数类型（functionType），表示无参数且返回类型为空的函数。
    llvm::Type* voidType = llvm::Type::getVoidTy(context);
    llvm::FunctionType* functionType = llvm::FunctionType::get(voidType, false);
    
    // 创建一个具有外部链接的函数，命名为"HelloFunction"并添加到模块中
    // llvm::GlobalValue::ExternalLinkage 指定创建的函数具有外部链接属性，
    // 表示该函数可以在外部被引用或链接。这意味着函数在链接阶段可以与其它对象文件或库中的同名函数进行链接。
    // llvm::GlobalValue 是 LLVM 中表示全局值（如函数、全局变量）的类。
    // GlobalValue 的成员 ExternalLinkage 表示该全局值具有外部链接属性，允许在不同编译单元之间进行链接。
    llvm::Function* function = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage,
                                                    "HelloFunction", module);
    
    // 验证函数，确保其内部结构的正确性
    llvm::verifyFunction(*function);
    
    // 打印模块内容到标准输出
    module->print(llvm::outs(), nullptr);

    return 0;
}
