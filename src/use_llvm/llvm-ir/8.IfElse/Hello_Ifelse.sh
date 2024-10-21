#!/bin/bash
LLVM_CONFIG=`llvm-config --cxxflags --ldflags --system-libs --libs core`

clang++ -w -v  $LLVM_CONFIG -fuse-ld=lld Hello_Ifelse.cc -o Hello_Ifelse

./Hello_Ifelse

rm ./Hello_Ifelse