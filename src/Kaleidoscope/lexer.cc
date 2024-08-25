/* 词法分析器(lexer) 识别文本内容
*  词法分析器（lexer）在解析文本时，如果遇到一个未知的字符，
*  它会返回一个介于0到255之间的整数值，这通常对应于字符的ASCII码。
*  如果遇到的是一个已知的符号或单词，词法分析器则会返回一个特定的标记（token），
*  这些标记是预定义的枚举值。
*/

// The lexer returns tokens [0-255] if it is an unknown character, otherwise one
// of these for known things.

#include <cctype>
#include <cstdio>
#include <cstdlib>
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

static int gettok() {
    static int LastChar = ' ';
    // 遇到空白字符串跳走
    while (std::isspace(LastChar)){
        LastChar = getchar();
    }
    // 识别[a-zA-z][a-zA-Z0-9]* 标识符号
    if(std::isalpha(LastChar)){
        IdentifierStr = LastChar;
        while (std::isalnum(LastChar = getchar())) {
            IdentifierStr += LastChar;
        }
        if (IdentifierStr == "def") {
            return tok_def;
        }
        if (IdentifierStr == "extern") {
            return tok_extern;
        }
        return tok_identifier;
    }
    // Number: [0-9]+ 识别数字
    if (std::isdigit(LastChar) || LastChar == '.') {
        std::string Number;
        do {
            Number += LastChar;
            LastChar = getchar();
        }while (std::isdigit(LastChar) || LastChar == '.');
        NumVal = strtod(Number.c_str(), 0);
        return tok_number;
    }
    // 判断注释
    if(LastChar == '#')
    {
        do {
            LastChar = getchar();
        }while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF) {
            return gettok();
        }
    }

    /* 处理未知的字符，如文件结束标志和操作符，
     * 词法分析器作为其ASCLL值返回
     * 或者文件结束时返回一个特定的标记
     */
    // 判断文件结束
    if (LastChar == EOF) {
        return tok_eof;
    }

    int ThisChar = LastChar;
    LastChar = getchar();

    return ThisChar;
}





