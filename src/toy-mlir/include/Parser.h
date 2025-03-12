#ifndef __TOY_PASER_H__
#define __TOY_PASER_H__

//===----------------------------------------------------------------------===//
// 该文件实现Toy语言的解析器，处理Lexer提供的Token流并生成AST。
//===----------------------------------------------------------------------===//

#include "Lexer.h"

namespace toy {

/// 梯度下降解析器：将Token流转换为AST (不进行语义检查，仅语法分析)
class Paeser {
public:
    Paeser(Lexer &lexer) : lexer(lexer) {} // 构造函数，传入Lexer引用

    /// 解析整个模块(一组函数定义)
    

private:

Lexer &lexer;


};

}




#endif //  __TOY_PASER_H__