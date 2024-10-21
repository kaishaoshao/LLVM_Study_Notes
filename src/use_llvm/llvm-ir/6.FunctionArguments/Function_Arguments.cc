// 假定这个函数带有两个参数，它们的名称分别为a和b，类型均为32位整数型

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Verifier.h"


#include <vector>
// 主函数
int main(int argc, char **argv)
{
    // 创建LLVM上下文，它贯穿于整个编译过程
    llvm::LLVMContext context;
    // 创建IR构建器，用于构造LLVM IR代码
    llvm::IRBuilder<> builder(context);
    
    // 创建一个LLVM模块，存放我们的IR代码
    llvm::Module *module = new llvm::Module("my_Module", context);
    
    // 在模块中获取或插入一个全局变量
    module->getOrInsertGlobal("my_global_variable", builder.getInt32Ty());
    // 获取这个全局变量的指针
    llvm::GlobalVariable* globalVariable = module->getNamedGlobal("my_global_variable");
    // 设置全局变量的链接类型
    globalVariable->setLinkage(llvm::GlobalValue::CommonLinkage);
    // 设置全局变量的对齐方式
    globalVariable->setAlignment(llvm::MaybeAlign(4));

    // 定义函数参数类型
    std::vector<llvm::Type*> parameters(2,builder.getInt32Ty());
    // 创建函数类型
    llvm::FunctionType *functionType = llvm::FunctionType::get(builder.getInt32Ty(),parameters,false);
    // 创建函数，并指定其链接类型和名称
    llvm::Function *function = llvm::Function::Create(functionType,llvm::GlobalValue::ExternalLinkage,
                                                         "my_function",module);

    // 为函数的参数命名，提高代码可读性
    function->getArg(0)->setName("a");
    function->getArg(1)->setName("b");
    
    // 创建函数的基本块，并设置为当前构建位置
    llvm::BasicBlock* block = llvm::BasicBlock::Create(context,"entry",function);
    builder.SetInsertPoint(block);

    // 创建一个常量整数，并返回这个值
    llvm::ConstantInt* zero = builder.getInt32(0);
    builder.CreateRet(zero);

    // 验证函数的正确性，确保IR代码生成无误
    llvm::verifyFunction(*function);
    // 打印模块的IR代码，便于查看生成结果
    module->print(llvm::outs(),nullptr);

    return 0;
}
