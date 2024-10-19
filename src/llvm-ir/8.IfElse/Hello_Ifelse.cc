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
    
    llvm
    
    return 0;
}