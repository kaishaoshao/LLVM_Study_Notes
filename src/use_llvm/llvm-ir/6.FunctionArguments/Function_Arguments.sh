#!/bin/bash
LLVM_CONFIG=`llvm-config --cxxflags --ldflags --system-libs --libs core`

clang++ -w -v  $LLVM_CONFIG -fuse-ld=lld Function_Arguments.cc -o Function_Arguments

./Function_Arguments

rm ./Function_Arguments