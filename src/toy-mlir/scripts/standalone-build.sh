#!/bin/bash
LLVM_BUILD_DIR=/Users/kaishaoshao/Desktop/triton/llvm-project/build
MLIR_DIR=${LLVM_BUILD_DIR}/lib/cmake/mlir
LLVM_EXTERNAL_LIT=${LLVM_BUILD_DIR}/bin/llvm-lit

cmake -G Ninja .. -DMLIR_DIR=$MLIR_DIR -DLLVM_EXTERNAL_LIT=$LLVM_EXTERNAL_LIT
cmake --build . --target toyc