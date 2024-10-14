#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"


// LLVMContext：提供了一个命名空间，用于创建和存储 LLVM IR 对象
// Module：代表整个编译单元的代码集合，可以包含函数、全局变量、别名和数据。
int main(int argc,char** argv){
    llvm::LLVMContext context;
    llvm::Module* module = new llvm::Module("Helo Module",context);

    module->print(llvm::outs(),nullptr);

    return 0;
}
