#include "../logger/logger.h"
#include <utility>
#include <map>
#define __PARSER_H__
#ifdef  __PARSER_H__

extern std::map<char,int> BinopPrecedence;  // 用于存出二元运算符的优先级4


std::unique_ptr<ExprAST> ParseExpression();

void HandleTopLevelExpression();

void HandleExtern();

void HandleDefinition();

#endif