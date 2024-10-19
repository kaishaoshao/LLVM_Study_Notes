// 这一章仍然是在处理if-else语句。不过，这一次我们用一个特殊的指令来生成IR代码，即phi指令。
// 一个if-else语句包含了一个条件判断以及两个逻辑分支。最终会运行哪个分支的代码，
// 取决于条件判断的结果为真还是假。而“条件”则一般是一个比较表达式。

// 在很多情况下，控制流只是为了给某一个变量赋值，而phi 指令，则可以根据控制流来选择合适的值。它的用法如下（示例）：
// %value = phi i32 [66, %branch1], [77, %branch2], [88, %branch3] 

// phi指令可以接收多个输入参数，参数的个数也不是固定的。第一个参数表示的是phi指令的返回值类型，如在以上示例中为i32。
// 接下来的每一个参数都是一个数组，代表了每一个分支及其对应的返回值。例如，如果前一步执行的是branch1分支，则返回值为66；
// 当执行的是branch2，则返回值为77；以此类推…
// LLVM也提供了相应的C++ API用于创建phi指令：
// PHINode * llvm::IRBuilderBase::CreatePHI(Type * Ty, unsigned NumReservedValues, const Twine & Name = "")
// 以及向phi指令中添加条件返回值：
// void llvm::PHINode::addIncoming(Value * V, BasicBlock * BB)

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

    // Create a module
    llvm::Module* module = new llvm::Module("Test.c", context);

    // add a function
    std::vector<llvm::Type*> parameters(1,builder.getInt32Ty());
    llvm::FunctionType* functionType = llvm::FunctionType::get(builder.getInt32Ty(),parameters,false);
    llvm::Function *function = llvm::Function::Create(functionType,llvm::GlobalValue::ExternalLinkage,
                                                        "Test",module);
    
    // add an argument to the function
    llvm::Value* arg = function->getArg(0);
    arg->setName("a");

        // Add some basic blocks to the function
    llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(context, "entry", function);
    llvm::BasicBlock* thenBlock = llvm::BasicBlock::Create(context, "if.then", function);
    llvm::BasicBlock* elseBlock = llvm::BasicBlock::Create(context, "if.else", function);
    llvm::BasicBlock* returnBlock = llvm::BasicBlock::Create(context, "if.end", function);

    // Fill the "entry" block(1)
    // int b
    builder.SetInsertPoint(entryBlock);
    llvm::Value *bPtr = builder.CreateAlloca(builder.getInt32Ty(),nullptr,"b.address");

    // Fill the "entry" block(2)
    // if (a > 33)
    llvm::Constant *value33 = builder.getInt32(33);
    llvm::Value *condition = builder.CreateICmpSGT(arg,value33,"compare.result");
    builder.CreateCondBr(condition,thenBlock,elseBlock);

    
    // Fill the "if.then" block:
    // b = 66
    builder.SetInsertPoint(thenBlock);
    llvm::ConstantInt *value66 = builder.getInt32(66);
    builder.CreateBr(returnBlock);

    // Fill the "if.else" block:
    // b = 77
    builder.SetInsertPoint(elseBlock);
    llvm::ConstantInt *value77 = builder.getInt32(77);
    builder.CreateBr(returnBlock);

    // Fill the "if.end" block with phi instruction:
    //   return b;
    builder.SetInsertPoint(returnBlock);
    llvm::PHINode *phi = builder.CreatePHI(builder.getInt32Ty(),2);
    phi->addIncoming(value66,thenBlock);
    phi->addIncoming(value77,elseBlock);
    builder.CreateStore(phi,bPtr);
    llvm::Value *returnValue = builder.CreateLoad(builder.getInt32Ty(),bPtr,"return,value");
    builder.CreateRet(returnValue);

    // print thr IR
    verifyFunction(*function);
    module->print(llvm::outs(),nullptr);


    return 0;
}
