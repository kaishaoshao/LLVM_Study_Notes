// Test.c
// int Test(int a)
// {
//     int b;
//     if (a > 33)
//     {
//         b = 66;
//     }
//     else
//     {
//         b = 77;
//     }

//     return b;
// }
// 可变变量b，当然它也是一个局部变量，我们可以在栈上创建一个可变变量。
// LLVM IR提供了一个指令，可以让我们在栈上申明变量，即 alloca 指令。
// %variable.address = alloca i32
// LLVM也提供了相应的C++ API用于在栈上申明变量：
// AllocaInst * llvm::IRBuilderBase::CreateAlloca(Type * Ty, Value * ArraySize = nullptr, const Twine & Name = "")
// 注意用alloca 指令申明的变量，其实得到是变量的地址。如果要访问它，我们需要用store和load指令。
// store指令可以把变量值写入该地址（示例）：
// store i32 66, i32* %variable.address, align 4
// 而load指令则可以把变量值从该地址的读取出来（示例）：
// %actual.value = load i32, i32* %variable.address, align 4


#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"

#include <vector>

int main(int argc,char **argv)
{
    llvm::LLVMContext context;
    llvm::IRBuilder<> builder(context);
    
    llvm::Module *module = new llvm::Module("Test.c",context);

    // Add a function
    std::vector<llvm::Type*> parameters(1,builder.getInt32Ty());
    llvm::FunctionType* functionType = llvm::FunctionType::get(builder.getInt32Ty(),parameters,false);
    llvm::Function* function = llvm::Function::Create(functionType,llvm::GlobalValue::ExternalLinkage,
                                                    "Test",module);
    // Add an argument to the function
    llvm::Value *arg = function->getArg(0);
    arg->setName("a");

    // Add some basic blocks to function
    llvm::BasicBlock *entryBlock = llvm::BasicBlock::Create(context,"entry",function);
    llvm::BasicBlock *thenBlock = llvm::BasicBlock::Create(context,"if.then",function);
    llvm::BasicBlock *elseBlock = llvm::BasicBlock::Create(context,"if.else",function);
    llvm::BasicBlock *returnBlock = llvm::BasicBlock::Create(context,"if.end",function);

    // Fill the "entry" block(1)
    // int b
    builder.SetInsertPoint(entryBlock);
    llvm::Value *bPtr = builder.CreateAlloca(builder.getInt32Ty(),nullptr,"b.address");

    // Fill the "entry" block(2)
    // if (a > 33)
    llvm::Constant *value33 = builder.getInt32(33);
    llvm::Value *condition = builder.CreateICmpSGT(arg,value33,"compare.result");
    builder.CreateCondBr(condition,thenBlock,elseBlock);
 

    // Fill the "if.end" block:
    // return b;
    builder.SetInsertPoint(returnBlock);
    llvm::Value *returnValue = builder.CreateLoad(builder.getInt32Ty(),bPtr,"return.value");
    builder.CreateRet(returnValue);

    // Print the IR
    verifyFunction(*function);
    module->print(llvm::outs(),nullptr);

    
    return 0;
}