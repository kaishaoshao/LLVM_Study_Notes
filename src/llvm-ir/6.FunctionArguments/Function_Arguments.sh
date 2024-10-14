#!/bin/bash
# Set up C++ standard library and header path
LLVM_CONFIG=`llvm-config --cxxflags --ldflags --system-libs --libs core`

clang++ -w -v  $LLVM_CONFIG -fuse-ld=lld Hello_Function_Arguments.cc -o Hello_Function_Arguments

./Hello_Function_Arguments

rm ./Hello_Function_Arguments