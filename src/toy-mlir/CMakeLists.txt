cmake_minimum_required(VERSION 3.20)
project(toy-mlir)

# 全局配置
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib) # 静态库输出目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib) # 动态库输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin) # 可执行文件输出目录

add_subdirectory(include)
add_subdirectory(paser)

# 测试库
# add_subdirectory(test)


find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")

# 包含 LLVM 头文件和定义
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# 映射所有需要的 LLVM 组件
llvm_map_components_to_libnames(llvm_libs
    Core          # 核心数据结构
    Support       # 基础工具（MemoryBuffer、errs() 等）
)

# MLIR 相关组件
get_property(dialect_libs GLOBAL PROPERTY MLIR_DIALECT_LIBS)
get_property(translation_libs GLOBAL PROPERTY MLIR_TRANSLATION_LIBS)

add_executable(toy-mlir toyc.cpp)

target_link_libraries(toy-mlir
PRIVATE
    AST_LIB             # paser模块生成的库
    ${llvm_libs}
    ${dialect_libs}
    ${translation_libs}
    MLIRIR
    MLIRParser
    MLIRPass
    MLIRSPIRVDialect
    MLIRTranslateLib
    MLIRSupport
#   ${GTEST_LIBRARIES}  # 测试库
)

mlir_check_link_libraries(toy-mlir)

