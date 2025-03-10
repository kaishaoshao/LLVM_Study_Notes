#include "Lexer.h"
#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    // 读取文件内容
    std::ifstream inputFile(argv[1], std::ios::binary);
    if (!inputFile) {
        std::cerr << "Error: Unable to open file " << argv[1] << std::endl;
        return 1;
    }

    // 将文件内容读入内存
    std::vector<char> fileContent((std::istreambuf_iterator<char>(inputFile)),
                                  std::istreambuf_iterator<char>());
    fileContent.push_back('\0');  // 添加字符串结尾符

    // 创建 LexerBuffer 实例
    toy::LexerBuffer lexer(fileContent.data(), fileContent.data() + fileContent.size(), argv[1]);

    // 获取第一个 Token
    toy::Token tok = lexer.getNextToken();

    // 循环解析 Token 并打印信息
    while (tok != toy::tok_eof) {
        std::cout << "Token: ";
        switch (tok) {
            case toy::tok_semicolon:
                std::cout << "semicolon (;)" << std::endl;
                break;
            case toy::tok_parenthese_open:
                std::cout << "parenthese open (" << std::endl;
                break;
            case toy::tok_parenthese_close:
                std::cout << "parenthese close )" << std::endl;
                break;
            case toy::tok_bracket_open:
                std::cout << "bracket open {" << std::endl;
                break;
            case toy::tok_bracket_close:
                std::cout << "bracket close }" << std::endl;
                break;
            case toy::tok_sbracket_open:
                std::cout << "sbracket open [" << std::endl;
                break;
            case toy::tok_sbracket_close:
                std::cout << "sbracket close ]" << std::endl;
                break;
            case toy::tok_return:
                std::cout << "return" << std::endl;
                break;
            case toy::tok_var:
                std::cout << "var" << std::endl;
                break;
            case toy::tok_def:
                std::cout << "def" << std::endl;
                break;
            case toy::tok_identifier:
                std::cout << "identifier: " << lexer.getId().str() << std::endl;
                break;
            case toy::tok_number:
                std::cout << "number: " << lexer.getNumber() << std::endl;
                break;
            default:
                std::cout << "unknown token" << std::endl;
                break;
        }

        // 获取下一个 Token
        tok = lexer.getNextToken();
    }

    return 0;
}