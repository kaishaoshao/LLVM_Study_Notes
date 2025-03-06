#/bin/bash

LLVM_SOURCE=/home/shaokai/desktop/code/llvm/mlir-tutorial_zh/src/llvm-project/

mkdir -p toy

# Import chapter 1.
rsync -rvz $LLVM_SOURCE/mlir/examples/toy/Ch1/include/toy/ toy/
rsync -rvz $LLVM_SOURCE/mlir/examples/toy/Ch1/parser/ toy/
rsync -rvz $LLVM_SOURCE/mlir/examples/toy/Ch1/toyc.cpp toy/