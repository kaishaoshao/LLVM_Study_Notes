#!/bin/bash

# 检查传递给脚本的参数
if [ "$1" == "--hello" ]; then
    ./chibicc -S ./test/hello.c
elif [ "$1" == "--function" ]; then
    ./chibicc -S ./test/function_llvm.c
else
    echo "Usage: $0 [--hello|--function]"
    exit 1
fi
rm ./chibicc