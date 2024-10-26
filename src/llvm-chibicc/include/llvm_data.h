#include "../chibicc.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"

#include <iostream>
#include <memory>

#define DUMP_OBJ 1

// llvm::LLVMContext 是 LLVM 中的一个类，它提供了一个上下文环境，
// 用于存储和关联各种 LLVM IR（中间表示）对象。
static std::unique_ptr<llvm::LLVMContext> TheContext;

// llvm::Module 是 LLVM 中的一个核心类，它代表了一个编译单元的整个代码集合。
// 在 LLVM 的中间表示（IR）中，Module 包含了函数、全局变量、别名、数据和指令等信息。
// 它通常用于表示一个完整的源文件编译后的结果，并且可以被用来生成机器代码或进行进一步的优化。
static std::unique_ptr<llvm::Module> TheModule;
static std::unique_ptr<llvm::IRBuilder<>>  Builder;

// void InitializeModele(const std::string &filename);

// llvm::LLVMContext &getLLVMContext();
// uint64_t read_integer_from_buf(const char *buf,int size);
// llvm::Constant *build_integer(llvm::Type *type, Type *ctype,
//                                     char *buf,int offset,Relocation *rel);
//                                     llvm::Constant *build_float(llvm::Type *type, Type *ctype,
//                                     char *buf,int offset,Relocation *rel);
// llvm::Constant *build_double(llvm::Type *type, Type *ctype,
//                                     char *buf,int offset,Relocation *rel);
//                                     llvm::Constant *build_constant(llvm::Type *type, Type *ctype,
//                                     char *buf,int offset,Relocation *rel);
// llvm::Constant *build_initializer(llvm::Type *type,Obj *var);
// llvm::Type *createLLVMType(Type *ty);


// void emit_global_var(Obj *var);
// void emit_data_llvm(Obj *prog);


