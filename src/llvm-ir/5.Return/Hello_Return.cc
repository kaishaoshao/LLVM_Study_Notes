#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"

// 主函数
int main(int argc, char **argv) {
    // 创建LLVM上下文，它贯穿于整个编译过程
    llvm::LLVMContext context;
    // 创建IR构建器，用于构造LLVM IR代码
    llvm::IRBuilder<> builder(context);
    
    // 创建一个模块，包含要编译的IR代码
    llvm::Module *module = new llvm::Module("hello_return", context);
    
    // 在模块中插入或获取一个全局变量
    module->getOrInsertGlobal("hello_GlobalVariable",llvm::Type::getInt32Ty(context));
    // 获取命名全局变量
    llvm::GlobalVariable* globalVariable = module->getNamedGlobal("hello_GlobalVariable");
    // 设置全局变量的链接类型
    globalVariable->setLinkage(llvm::GlobalValue::CommonLinkage);
    // 设置全局变量的对齐方式
    globalVariable->setAlignment(llvm::MaybeAlign(4));
    
    // 定义空类型
    llvm::Type *voidType = llvm::Type::getVoidTy(context);
    // 定义函数类型
    llvm::FunctionType* functionType = llvm::FunctionType::get(voidType,false);
    // 创建函数
    llvm::Function* function = llvm::Function::Create(functionType,llvm::GlobalValue::ExternalLinkage,
                                                        "hello_Function",module);

    // 创建基本块
    llvm::BasicBlock* block = llvm::BasicBlock::Create(context, "entry", function);
    // 设置构建器的插入点
    builder.SetInsertPoint(block);

        
    // 返回
    llvm::ConstantInt* zero = builder.getInt32(0);
    builder.CreateRet(zero);

   
    // 验证函数
    verifyFunction(*function);
    // 打印模块
    module->print(llvm::outs(),nullptr);

    return 0;
}


