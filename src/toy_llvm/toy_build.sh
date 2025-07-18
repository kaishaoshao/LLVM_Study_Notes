#!/bin/bash
if [[ -d build ]]; then
    ninja -C build llc
else
    mkdir build
    pushd build
    cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang \
          -DLLVM_TARGETS_TO_BUILD=Toy -DLLVM_ENABLE_PROJECTS="clang" \
          -DLLVM_OPTIMIZED_TABLEGEN=On -DLLVM_PARALLEL_LINK_JOBS=1 -G "Ninja" ../llvm
    ninja llc
    popd
fi