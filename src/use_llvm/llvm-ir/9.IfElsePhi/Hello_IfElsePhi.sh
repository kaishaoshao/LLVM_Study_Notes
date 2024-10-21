#!/bin/bash
LLVM_CONFIG=`llvm-config --cxxflags --ldflags --system-libs --libs core`

clang++ -w -v  $LLVM_CONFIG -fuse-ld=lld Hello_ifElsePhi.cc -o Hello_ifElsePhi

./Hello_ifElsePhi

rm ./Hello_ifElsePhi