#!/bin/bash
#

MLIR_PATH=/home/shaokai/desktop/code/llvm/mlir-tutorial_zh/src/llvm-project/build/bin

export PATH="$MLIR_PATH:$PATH"


toyc-ch1 examples/basic.toy -emit=ast