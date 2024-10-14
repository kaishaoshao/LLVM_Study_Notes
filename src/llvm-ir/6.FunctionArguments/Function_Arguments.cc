// 假定这个函数带有两个参数，它们的名称分别为a和b，类型均为32位整数型

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Function.h"

#include <vector>

int main(int argc, char **argv)
{
    llvm::LLVMContext context;
    llvm::IRBuilder<> builder(context);
    
    llvm::Module *module = new llvm::Module("my_Module", context);
    
    module->getOrInsertGlobal("my_global_variable", builder.getInt32Ty());
    llvm::GlobalVariable* globalVariable = module->getNamedGlobal("my_global_variable");
    globalVariable->setLinkage(llvm::GlobalValue::CommonLinkage);
    globalVariable->setInitializer(llvm::MaybeAlign(4));

    std::vector<llvm::Type> parameters(2,builder.getInt32Ty());
    llvm::FunctionType *functionType = llvm::FunctionType::get(builder.getInt32Ty(),parameters,false);
    llvm::Function *function = llvm::Function::Create(functionType,llvm::GlobalVariable::ExternalLinkage
    llvm::Function *function = llvm::Function::Create(functionType,llvm::Function::ExternalLinkage,
                                                        "my_function",module);)
                                                    
    function->getArg(0)->setName("a");
    function->getArg(1)->setName("b");
    
    llvm::BasicBlock* block = llvm::BasicBlock::Create(context,"entry",function);
    

    return 0;
}