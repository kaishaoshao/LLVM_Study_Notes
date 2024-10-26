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

static uint64_t read_integer_from_buf(const char *buf,int size){
    switch (size)
    {
    case 1:
        return *buf;
    case 2:
        return *(uint16_t*)buf;
    case 4:
        return *(uint32_t*)buf;
    case 8:
        return *(uint64_t*)buf;
    default:
        return *buf;
    }
}

static llvm::Constant *build_integer(llvm::Type *type, Type *ctype,
                                    char *buf,int offset,Relocation *rel){
    if(!buf)
        return llvm::Constant::getNullValue(type); 
    return llvm::ConstantInt::get(type,read_integer_from_buf(buf+offset,ctype->size));
}

static llvm::Constant *build_float(llvm::Type *type, Type *ctype,
                                    char *buf,int offset,Relocation *rel){
    if(!buf)
        return llvm::Constant::getNullValue(type); 
    float f_val = *(float *)(buf + offset);
    return llvm::ConstantFP::get(getLLVMContext(),llvm::APFloat(f_val));
}

static llvm::Constant *build_double(llvm::Type *type, Type *ctype,
                                    char *buf,int offset,Relocation *rel){
    if(!buf)
        return llvm::Constant::getNullValue(type); 
    double d_val = *(double *)(buf + offset);
    return llvm::ConstantFP::get(getLLVMContext(),llvm::APFloat(d_val));
}


// llvm::Constant 是 LLVM 中的一个类，用于表示编译时已知的常量值。
// 这些值可以是整数、浮点数、字符串、null 指针等。
static llvm::Constant *build_constant(llvm::Type *type, Type *ctype,
                                    char *buf,int offset,Relocation *rel){
    llvm::Constant *constant = nullptr;
    // int size = ctype->size;
    switch (ctype->kind)                
    {
    case TY_CHAR:
    case TY_SHORT:
    case TY_INT:
    case TY_LONG:
          constant = build_integer(type,ctype,buf,offset,rel);
        break;
    case TY_FLOAT:
        constant = build_float(type,ctype,buf,offset,rel);
        break;
    
    case TY_DOUBLE:
        constant = build_double(type,ctype,buf,offset,rel);
        break;
    
    default:
        // 指针数组，后面实现 
        constant = Builder->getInt32(-1024);
        break;
    }
    return constant;
}

static llvm::Constant *build_initializer(llvm::Type *type,Obj *var)
{
    llvm::Constant *constant = build_constant(type,var->ty,var->init_data,0,var->rel);
    return constant;
}

// 将获取的c语言类型转换为llvm类型
static llvm::Type *createLLVMType(Type *ty){
    llvm::Type *llvm_type;
    switch (ty->kind)
    {
    case TY_CHAR:
        llvm_type = Builder->getInt8Ty();
        break;
    case TY_SHORT:
        llvm_type = Builder->getInt16Ty();
        break;
    case TY_INT:
        llvm_type = Builder->getInt32Ty();
        break;
    case TY_LONG:
        llvm_type = Builder->getInt32Ty();
        break;
    case TY_FLOAT:
        llvm_type = Builder->getFloatTy();
        break;
    case TY_DOUBLE:
        llvm_type = Builder->getDoubleTy();
        break;
    default:
        llvm_type = Builder->getInt32Ty();
        break;
    }
    return llvm_type;
}

void emit_global_var(Obj *var){
    Type *ty = var->ty;
    // 是函数类型
    if(ty->kind == TY_FUNC)
        return;
    std::string var_name = var->name;
    llvm::Type *llvm_type = createLLVMType(ty);
    TheModule->getOrInsertGlobal(var_name,llvm_type);
    llvm::GlobalVariable *gvar = TheModule->getNamedGlobal(var_name);
    llvm::Constant *initializer = build_initializer(llvm_type,var);
    gvar->setInitializer(initializer);

}
static void emit_data_llvm(Obj *prog){
    // 逆序生成
    if(!prog)
        return;
    emit_data_llvm(prog->next);
    emit_global_var(prog); 
}

void gen_ir(Obj *prog,const std::string &filename){
    InitializeModele(filename);
    if(DUMP_OBJ)
        dump_obj(prog);
    emit_data_llvm(prog);
   // emit_function();
    TheModule->print(llvm::outs(),nullptr);
}