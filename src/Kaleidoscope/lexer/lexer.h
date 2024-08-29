#define __LEXER_H__
#ifdef __LEXER_H__
#include <string>

enum ToKen{
    tok_eof = -1,           // 文件结束

    tok_def = -2,           // 定义关键字
    tok_extern = -3,        // 外部关键字

    tok_identifier = -4,    // 标识符
    tok_number = -5         // 数字
};
// 存储识别出的标识符字符串 当词法分析器返回tok_identifier标记时，这个字符串会被填充。
static std::string IdentifierStr;  
// 存储识别出的数字值，当词法分析器返回tok_number标记时，这个值会被填充。
static double NumVal;

static int CurTok;

int getNextToken(); 

#endif
