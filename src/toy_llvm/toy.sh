#!/bin/bash
cp ./toy_llvm/include/llvm/TargetParser/Triple.h ./llvm/include/llvm/TargetParser/


cp ./toy_llvm/lib/Target/Toy/ ./llvm/lib/Target -rf
cp ./toy_llvm/CMakeLists.txt ./llvm/

#!/bin/bash
if [[ -d build ]]; then
    ninja -C build llc
else
    mkdir build
    pushd build
    cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang \
          -DLLVM_TARGETS_TO_BUILD=Toy -DLLVM_ENABLE_PROJECTS="clang" \
          -DLLVM_OPTIMIZED_TABLEGEN=On -DLLVM_PARALLEL_LINK_JOBS=1  ../llvm
    make -j4 llc
    popd
fi