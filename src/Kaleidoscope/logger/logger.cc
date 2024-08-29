#include "logger.h"

// 用于错误处理的小助手函数
std::unique_ptr<ExprAST> LogError(const char *Str){ 
    fprintf(stderr,"Error: %s\n",Str);  
    return nullptr; 
}

std::unique_ptr<PrototypeAST> LogErrorP(const char *Str){
    LogError(Str);
    return nullptr;
}