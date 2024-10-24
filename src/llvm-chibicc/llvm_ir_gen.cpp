// 创建 LLVM Module

#include "chibicc.h"
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


static void print_type(Type *type){
    if(!type)
        return;
    std::cout<< "Type Kind: " << ctypeKindString(type->kind)
             << " align: " << type->align
             << std::endl;
    std::cout<< "*****************************"<<std::endl;
}

static void print_obj(Obj *obj){
    if(!obj)
        return;
    std::cout<< "Obj  name: " << obj->name
             << "\t  align: " << obj->align
             << std::endl;
    print_type(obj->ty);    
            
}

static void dump_obj(Obj *obj){
    for(Obj *cur = obj;cur;cur = cur->next){
        print_obj(cur);
    }
}

// 这里有问题没有想明白
static void InitializeModele(const std::string &filename){
    TheContext = std::make_unique<llvm::LLVMContext>();
    TheModule = std::make_unique<llvm::Module>(filename, *TheContext);
    Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext); 
}

static llvm::LLVMContext &getLLVMContext(){
    return TheModule->getContext();
}

// 将获取的c语言类型转换为llvm类型
static llvm::Type *createLLVMType(Type *ty){
    llvm::Type *llvm_type;
    switch (ty->kind)
    {
    case TY_CHAR:
        type = Builder->getInt8Ty();
        break;
    case TY_SHORT:
        type = Builder->getInt16Ty();
        break;
    case TY_INT:
        type = Builder->getInt32Ty();
        break;
    case TY_LONG:
        type = Builder->getInt32Ty();
        break;
    case TY_FLOAT:
        type = Builder->getFloatTy();
        break;
    case TY_DOUBLE:
        type = Builder->getDoubleTy();
        break;
    default:
        type = Builder->getInt32Ty();
        break;
    }
    return llvm_type;
}

static void emit_global_var(Obj *var){
    Type *ty = var->ty;
    // 是函数类型
    if(ty->kind == TY_FUNC)
        return;
    std::string var_name = var->name;
    llvm::Type *llvm_type = createLLVMType(ty);
    TheModule->getOrInsertGlobal(var_name,llvm_type);
    
}

static void emit_data(Obj *prog){
    // 逆序生成
    if(!prog)
        return;
    emit_data(prog->next);
    emit_global_var(prog); 
}

void gen_ir(Obj *prog,const std::string &filename){
    InitializeModele(filename);
    if(DUMP_OBJ)
        dump_obj(prog);
    emit_data(prog);
    TheModule->print(llvm::outs(),nullptr);
}