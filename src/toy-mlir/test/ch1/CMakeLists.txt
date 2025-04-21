# 单个测试用例构建
add_executable(test_Lexer
    test_Lexer.cpp
    ${CMAKE_SOURCE_DIR}/paser/AST.cpp
)

target_link_libraries(test_Lexer PRIVATE
    AST_LIB
    GTest::GTest
)

add_test(NAME lexer_test COMMAND test_Lexer)