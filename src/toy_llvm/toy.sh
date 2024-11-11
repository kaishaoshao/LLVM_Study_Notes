#!/bin/bash
cp ./toy_llvm/cmake/config-ix.cmake ./llvm/cmake -f
cp ./toy_llvm/cmake/modules/TableGen.cmake ./llvm/cmake/modules/

cp ./toy_llvm/include/llvm/BinaryFormat/ELF.h ./llvm/include/llvm/BinaryFormat/ 
cp ./toy_llvm/include/llvm/BinaryFormat/ELFRelocs/TOYRISCV.def ./llvm/include/llvm/BinaryFormat/ELFRelocs
cp ./toy_llvm/include/llvm/Object/ELFObjectFile.h ./llvm/include/llvm/Object/
cp ./toy_llvm/include/llvm/TargetParser/Triple.h ./llvm/include/llvm/TargetParser/

cp ./toy_llvm/lib/MC/MCSubtargetInfo.cpp ./llvm/lib/MC/
 cp ./toy_llvm/lib/Object/ELF.cpp ./llvm/lib/Object

cp ./toy_llvm/lib/Target/TOYRISCV/ ./llvm/lib/Target -rf
cp ./toy_llvm/CMakeLists.txt ./llvm/

mkdir build
cd build
rm * -rf
cmake -DCMAKE_BUILD_TYPE=Debug         \
        -DBUILD_SHARED_LIBS=ON         \
        -DLLVM_USE_LINKER=lld          \
        -DLLVM_ENABLE_PROJECTS="clang" \
        -DLLVM_TARGETS_TO_BUILD="TOYRISCV"   \
        -DLLVM_OPTIMIZED_TABLEGEN=ON   \
        ../llvm && make -j8