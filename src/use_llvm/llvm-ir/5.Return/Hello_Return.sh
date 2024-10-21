#!/bin/bash
# Set up C++ standard library and header path
LLVM_CONFIG=`llvm-config --cxxflags --ldflags --system-libs --libs core`

clang++ -w -v  $LLVM_CONFIG -fuse-ld=lld Hello_Return.cc -o Hello_Return

./Hello_Return

rm ./Hello_Return