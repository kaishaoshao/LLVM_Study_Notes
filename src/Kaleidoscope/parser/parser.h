#include "../logger/logger.h"
#include <utility>
#include <map>
#define __PARSER_H__
#ifdef  __PARSER_H__

static std::map<char,int> BinopPrecedence;  // 用于存出二元运算符的优先级4


static std::unique_ptr<ExprAST> ParseExpression();

static void HandleTopLevelExpression();

static void HandleExtern();

static void HandleDefinition();

#endif