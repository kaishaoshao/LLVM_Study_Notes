#!/bin/bash
# Set up C++ standard library and header path
LLVM_CONFIG=`llvm-config --cxxflags --ldflags --system-libs --libs core`

clang++ -w -v  $LLVM_CONFIG -fuse-ld=lld Hello_Block.cc -o Hello_Block

./Hello_Block

rm ./Hello_Block