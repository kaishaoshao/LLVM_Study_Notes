#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Verifier.h"

#include <vector>

// 主函数
int main(int argc, char **argv) {
    // 创建LLVM上下文对象，用于存储全局状态
    llvm::LLVMContext context;
    // 创建IR构建器，用于生成LLVM IR代码
    llvm::IRBuilder<> builder(context);
    
    // 创建一个模块对象，用于存储IR代码
    llvm::Module* module = new llvm::Module("my_module", context);
    
    // 在模块中获取或插入一个全局变量，并指定其类型为32位整数
    module->getOrInsertGlobal("my_global_var", builder.getInt32Ty());
    // 获取名为"my_global_var"的全局变量
    llvm::GlobalVariable* globalVariable = module->getNamedGlobal("my_global_var");
    // 设置全局变量的链接类型为CommonLinkage
    globalVariable->setLinkage(llvm::GlobalValue::CommonLinkage);
    // 设置全局变量的对齐属性为4字节对齐
    globalVariable->setAlignment(llvm::MaybeAlign(4));

    // 创建函数参数类型列表，包含两个32位整数类型
    std::vector<llvm::Type*> parameters(2, builder.getInt32Ty());
    // 根据参数类型列表创建函数类型，返回类型为32位整数
    llvm::FunctionType* functionType = llvm::FunctionType::get(builder.getInt32Ty(), parameters, false);
    // 创建一个外部链接的函数，并命名为"my_function"
    llvm::Function* function = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage,
                                                     "my_function", module);
    
    // 为函数的第一个参数命名"a"
    function->getArg(0)->setName("a");
    // 为函数的第二个参数命名"b"
    function->getArg(1)->setName("b");
    
    // 创建函数的基本块，并将其插入到函数的末尾
    llvm::BasicBlock *block = llvm::BasicBlock::Create(context, "entry", function);
    // 设置IR构建器的插入点为新创建的基本块
    builder.SetInsertPoint(block);
    
    // 获取函数的第一个参数
    llvm::Value *arg1 = function->getArg(0);
    // 创建一个常量值，值为3
    llvm::Constant *three = builder.getInt32(3);
    // 创建乘法指令，将第一个参数与常量3相乘，并为结果命名"multiplyResult"
    llvm::Value *result = builder.CreateMul(arg1, three, "multiplyResult");
       
    // 创建返回指令，返回乘法结果
    builder.CreateRet(result);
    
    // 验证函数的正确性
    verifyFunction(*function);
    // 将模块的IR代码输出到标准输出流
    module->print(llvm::outs(), nullptr);
    
    return 0;
}

