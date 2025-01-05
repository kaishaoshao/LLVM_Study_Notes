#!/bin/bash

# cmake
cp ./cpu0_llvm12/llvm/modify/llvm/cmake/config-ix.cmake ./llvm/cmake -f
# cp ./cpu0_llvm12/llvm/modify/llvm/cmake/modules/TableGen.cmake ./llvm/cmake/modules/

# include
cp ./cpu0_llvm12/llvm/modify/llvm/include/llvm/ADT/Triple.h ./llvm/include/llvm/ADT/
cp ./cpu0_llvm12/llvm/modify/llvm/include/llvm/BinaryFormat/ELF.h ./llvm/include/llvm/BinaryFormat/ 
cp ./cpu0_llvm12/llvm/modify/llvm/include/llvm/BinaryFormat/ELFRelocs/Cpu0.def ./llvm/include/llvm/BinaryFormat/ELFRelocs
cp ./cpu0_llvm12/llvm/modify/llvm/include/llvm/Object/ELFObjectFile.h ./llvm/include/llvm/Object/

# lib
cp ./cpu0_llvm12/llvm/modify/llvm/lib/MC/MCSubtargetInfo.cpp ./llvm/lib/MC/
cp ./cpu0_llvm12/llvm/modify/llvm/lib/Object/ELF.cpp ./llvm/lib/Object
cp ./cpu0_llvm12/llvm/modify/llvm/lib/Support/Triple.cpp ./llvm/lib/Support
cp ./cpu0_llvm12/llvm/modify/llvm/lib/Target/Cpu0/ ./llvm/lib/Target -rf
cp ./cpu0_llvm12/llvm/modify/llvm/CMakeLists.txt ./llvm/

mkdir build
cd build
rm * -rf
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang \
  -DLLVM_TARGETS_TO_BUILD=Cpu0 \
  -DLLVM_OPTIMIZED_TABLEGEN=On  \
  -DLLVM_PARALLEL_COMPILE_JOBS=16 -DLLVM_PARALLEL_LINK_JOBS=2 -G "Ninja" ../llvm
  time ninja -j8