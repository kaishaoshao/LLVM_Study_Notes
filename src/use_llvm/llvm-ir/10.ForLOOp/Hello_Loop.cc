// 一个for语句包含了条件判断以及循环执行的任务。具体循环多少次，取决于条件判断的结果。  
// 而“条件”则包含了循环的起点、终止条件、步进长度。
// 这里的for循环语句IR，其实跟前面章节的if-else语句有些相似的地方。
// 它们都需要用到比较指令、跳转指令、栈上的变量等。
// Test.c
// int Test(int a)
// {
//     int b = 0;
//     for (int i = 0; i < a; i++)
//     {
//         b = b + i;
//     }
//     return b;
// }
// 可变变量b，我们可以用alloca 指令在栈上申明一个局部变量b。注意用alloca 指令申明的变量，其实得到是变量的地址。
// 如果要访问它，我们需要用store和load指令。

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

    llvm::Module* module = new llvm::Module("Test.c", context);

    
        // Add a function with argument
    //   int Test(int a)
    std::vector<llvm::Type*> parameters(1, builder.getInt32Ty());
    llvm::FunctionType* functionType = llvm::FunctionType::get(builder.getInt32Ty(), parameters, false);
    llvm::Function* function = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, 
                                "Test", module);
    llvm::Value* a = function->getArg(0);
    a->setName("a");

    // Add some basic blocks to the function
    llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(context, "entry", function);
    llvm::BasicBlock* forConditionBlock = llvm::BasicBlock::Create(context, "for.condition", function);
    llvm::BasicBlock* forBodyBlock = llvm::BasicBlock::Create(context, "for.body", function);
    llvm::BasicBlock* forIncrementBlock = llvm::BasicBlock::Create(context, "for.increment", function);
    llvm::BasicBlock* forEndBlock = llvm::BasicBlock::Create(context, "for.end", function);

    // Fill the "entry" block (1):
    //   int b = 0;
    builder.SetInsertPoint(entryBlock);
    llvm::Value* bPtr = builder.CreateAlloca(builder.getInt32Ty(), nullptr, "b.address");
    builder.CreateStore(builder.getInt32(1), bPtr);

    // Fill the "entry" block (2):
    //   for (int i = 0; ...)
    llvm::Value* iPtr = builder.CreateAlloca(builder.getInt32Ty(), nullptr, "i.address");
    builder.CreateStore(builder.getInt32(0), iPtr);
    builder.CreateBr(forConditionBlock);

    // Fill the "for.condition" block:
    //   for (... i < a; ...)
    builder.SetInsertPoint(forConditionBlock);
    llvm::Value* i0 = builder.CreateLoad(builder.getInt32Ty(),iPtr);
    llvm::Value* forConditionCompare = builder.CreateICmpSLT(i0, a, "for.condition.compare.result");
    builder.CreateCondBr(forConditionCompare, forBodyBlock, forEndBlock);

        // Fill the "for.body" block:
    //   b = b + i;
    builder.SetInsertPoint(forBodyBlock);
    llvm::Value* b0 = builder.CreateLoad(builder.getInt32Ty(),bPtr);
    llvm::Value* i1 = builder.CreateLoad(builder.getInt32Ty(),iPtr);
    llvm::Value* addResult = builder.CreateAdd(b0, i1, "add.result");
    builder.CreateStore(addResult, bPtr);
    builder.CreateBr(forIncrementBlock);

    // Fill the "for.increment" block:
    //   for (... i++)
    builder.SetInsertPoint(forIncrementBlock);
    llvm::Value* i2 = builder.CreateLoad(builder.getInt32Ty(),iPtr);
    llvm::Value* incrementedI = builder.CreateAdd(i2, builder.getInt32(1), "i.incremented");
    builder.CreateStore(incrementedI, iPtr);
    builder.CreateBr(forConditionBlock);

    // Fill the "for.end" block:
    //   return b;
    builder.SetInsertPoint(forEndBlock);
    llvm::Value* returnValue = builder.CreateLoad(builder.getInt32Ty(),bPtr, "return.value");
    builder.CreateRet(returnValue);

    // Print the IR
    verifyFunction(*function);
    module->print(llvm::outs(), nullptr);


    return 0;
}
