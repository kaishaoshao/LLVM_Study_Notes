cmake_minimum_required(VERSION 3.20)
project(toy-mlir)

# 全局配置
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib) # 静态库输出目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib) # 动态库输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin) # 可执行文件输出目录
 


add_subdirectory(include)
add_subdirectory(paser)
# add_subdirectory(test)

add_executable(toy-mlir toyc.cpp)

target_link_libraries(toy-mlir PRIVATE 
    AST_LIB     # paser模块生成的库
  #  ${GTEST_LIBRARIES}  # 测试库
)



