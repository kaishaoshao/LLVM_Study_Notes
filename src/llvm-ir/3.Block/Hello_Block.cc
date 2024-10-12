// 函数(Function)是由基本逻辑代码块(BasicBlock)组成的。
// 我们把“基本逻辑代码块”简称为“代码块”吧。一个代码块有且只有一个起点和一个终点。
// 代码块的主要部分当然是代码，即一系列的IR指令。代码块的起点是一个标签，它是整个代码块的标签。
// 标签的下一行就是第一条IR指令，而最后一条IR指令就是结束指令。
// LLVM中的BasicBlock类，就是用来创建、使用、处理代码块的。
// 我们还可以使用LLVM提供的IR代码创建工具（类）IRBuilder，它能让我们更方便快捷的创建出代码块。

// 1.llvm/IR/BasicBlock.h：
// 包含了 BasicBlock 类的定义，该类代表 LLVM IR 中的一个基本块。
// 基本块是一系列有序的指令集合，其中每个指令只能被顺序执行，且其中只能有一个分支指令
//（如 br, ret, switch 等）作为结束。

// llvm/IR/IRBuilder.h：
// 提供了 IRBuilder 类，这是一个方便的辅助类，用于在 LLVM IR 中创建指令。
// IRBuilder 管理当前的插入点（即下一个指令应该插入到哪里），并提供了许多工厂方法来创建不同类型的指令。

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"

int main(int argc,char** argv){
    llvm::LLVMContext context;
    llvm::IRBuilder<> builder(context);
    
    llvm::Module* module = new llvm::Module("my_Module",context);
    
    // 创建一个函数
    llvm::Type* voidType = llvm::Type::getVoidTy(context);
    llvm::FunctionType* functionType = llvm::FunctionType::get(voidType,false);
    llvm::Function* function = llvm::Function::Create(functionType,llvm::GlobalValue::ExternalLinkage
                                                        ,"my_Function",module);
    
    // 创建一个基本块
    llvm::BasicBlock* block = llvm::BasicBlock::Create(context,"Hello_Block",function);
    builder.SetInsertPoint(block);

    // 打印IR
    llvm::verifyFunction(*function);
    module->print(llvm::outs(),nullptr);
    
    return 0;
}


