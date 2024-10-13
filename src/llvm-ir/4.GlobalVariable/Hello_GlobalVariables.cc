// 全局变量(Global Variable)是在一个模块(Module)之内全局可见的变量，也就是说模块内所有的函数都能用它。
// LLVM提供了Module::getOrInsertGlobal()函数来创建全局变量，以及Module::getNamedGlobal()来找到一个全局变量。
// 创建全局变量之后，我们可以配置它的属性，如链接类型、内存对齐模型等。
// 全局变量在链接的时候，到底是指向同一个全局变量，还是多个不同的全局变量，是由链接类型决定的。
// 这里说的“多个不同的全局变量”，意思是其名称相同，但是有多个“分身”( 可以简单地理解为Copy、Instance、实例等等)，
// “分身”之间互不影响。

// ExternalLinkage               |  模块外部可见函数
// AvailableExternallylinkage    |  只在查询
// LinkOnceAnyLinkage	         |  函数在链接（或内连inline）的时候仅保存一份实现
// LinkOnceODRLinkage	         |  同上，但有些链接属性可以用类似的属性替换
// WeakAnyLinkage	             |  函数在链接的时候仅保存一份实现（弱链接）
// WeakODRLinkage	             |  同上，但有些链接属性可以用类似的属性替换
// AppendingLinkage	             |  这是特殊的链接类型，只适用于全局数组
// InternalLinkage	             |  链接时若发现有重命名的静态函数，则进行重命名
// PrivateLinkage	             |  只做内部链接，但不会添加到符号表中
// ExternalWeakLinkage	         |  外部弱链接
// CommonLinkage	             |  由编译器（链接器）自动选择链接类型

// llvm/IR/GlobalVariable.h：
// 这个头文件定义了GlobalVariable类，它代表LLVM IR中的全局变量。
// 全局变量是程序中所有函数都可以访问的变量，它们在程序的整个生命周期内都存在。
// GlobalVariable类提供了多种方法来设置和获取全局变量的属性.
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"

using namespace llvm;
 
int main(int argc,char **argv){
    llvm::LLVMContext context;
    llvm::IRBuilder<> builder(context);
    
    llvm::Module* module = new llvm::Module("my_Module",context);
    
    module->getOrInsertGlobal("hello_GlobalVariable",llvm::Type::getInt32Ty(context));
    llvm::GlobalVariable *globalVariable  = module->getNamedGlobal("my_GlobalVariable");
    globalVariable->setLinkage(llvm::GlobalValue::CommonLinkage);
    globalVariable->setAlignment(llvm::MaybeAlign(4));

    llvm::Type* voidType = llvm::Type::getVoidTy(context);
    llvm::FunctionType* functionType = llvm::FunctionType::get(voidType,false);
    llvm::Function* function = llvm::Function::Create(functionType,llvm::GlobalValue::ExternalLinkage,
                                "my_Function",module);

    llvm::BasicBlock *block = llvm::BasicBlock::Create(context,"entry",function);
    builder.SetInsertPoint(block);


    verifyFunction(*function);
    module->print(llvm::outs(),nullptr);

    return 0;
}

