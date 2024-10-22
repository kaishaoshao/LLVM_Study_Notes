#!/bin/bash
LLVM_CONFIG=`llvm-config --cxxflags --ldflags --system-libs --libs core`

clang++ -w -v  $LLVM_CONFIG -fuse-ld=lld Hello_Loop.cc -o Hello_Loop

./Hello_Loop

rm ./Hello_Loop