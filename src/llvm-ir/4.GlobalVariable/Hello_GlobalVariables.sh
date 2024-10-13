#!/bin/bash
# Set up C++ standard library and header path
LLVM_CONFIG=`llvm-config --cxxflags --ldflags --system-libs --libs core`

clang++-18 -w -v  $LLVM_CONFIG -fuse-ld=lld Hello_GlobalVariables.cc -o Hello_GlobalVariables

./Hello_GlobalVariables

rm ./Hello_GlobalVariables