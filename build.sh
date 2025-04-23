#!/bin/bash

# 请将脚本放置在llvm-project根目录下执行

# 获取当前脚本的绝对路径
SCRIPT_PATH="$(readlink -f "$0")"
# 获取脚本所在的目录
SCRIPT_DIR="$(dirname "$SCRIPT_PATH")"

# 判断当前目录下是否有 build 目录，没有则新建
if [ ! -d "$SCRIPT_DIR/build" ]; then
    mkdir "$SCRIPT_DIR/build" || exit
fi

# 配置 CMake 并生成构建文件
# -DCMAKE_BUILD_TYPE=DebugRelease 
cmake -B "$SCRIPT_DIR/build"           \
    -DCMAKE_BUILD_TYPE=Release         \
    -DBUILD_SHARED_LIBS=ON             \
    -DLLVM_USE_LINKER=lld              \
    -DLLVM_ENABLE_PROJECTS="clang;lld;lldb;mlir;clang-tools-extra;compiler-rt;" \
    -DCMAKE_C_COMPILER=clang           \
    -DCMAKE_CXX_COMPILER=clang++       \
    -DLLVM_TARGETS_TO_BUILD="X86;RISCV;ARM"   \
    -DLLVM_OPTIMIZED_TABLEGEN=ON       \
    -DLLVM_PARALLEL_COMPILE_JOBS=7     \
    -DLLVM_PARALLEL_LINK_JOBS=3        \
    -DLLVM_CCACHE_BUILD=ON             \
    -DLLVM_ENABLE_ASSERTIONS=ON        \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -GNinja "$SCRIPT_DIR/llvm"         \
    && ninja -C "$SCRIPT_DIR/build" -j8




	