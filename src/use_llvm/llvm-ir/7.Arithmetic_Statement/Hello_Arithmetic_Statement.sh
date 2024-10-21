#!/bin/bash
LLVM_CONFIG=`llvm-config --cxxflags --ldflags --system-libs --libs core`

clang++ -w -v  $LLVM_CONFIG -fuse-ld=lld Hello_Arithmetic_Statement.cc -o Hello_Arithmetic_Statement

./Hello_Arithmetic_Statement

rm ./Hello_Arithmetic_Statement