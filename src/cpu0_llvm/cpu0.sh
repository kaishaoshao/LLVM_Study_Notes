#!/bin/bash
cp ./cpu0_llvm/cmake/config-ix.cmake ./llvm/cmake -f
cp ./cpu0_llvm/cmake/modules/TableGen.cmake ./llvm/cmake/modules/

cp ./cpu0_llvm/include/llvm/BinaryFormat/ELF.h ./llvm/include/llvm/BinaryFormat/ 
cp ./cpu0_llvm/include/llvm/BinaryFormat/ELFRelocs/Cpu0.def ./llvm/include/llvm/BinaryFormat/ELFRelocs
cp ./cpu0_llvm/include/llvm/Object/ELFObjectFile.h ./llvm/include/llvm/Object/
cp ./cpu0_llvm/include/llvm/TargetParser/Triple.h ./llvm/include/llvm/TargetParser/

cp ./cpu0_llvm/lib/MC/MCSubtargetInfo.cpp ./llvm/lib/MC/
 cp ./cpu0_llvm/lib/Object/ELF.cpp ./llvm/lib/Object

cp ./cpu0_llvm/lib/Target/Cpu0/ ./llvm/lib/Target -rf
cp ./cpu0_llvm/CMakeLists.txt ./llvm/

cd build
rm * -rf
cmake -DCMAKE_BUILD_TYPE=Debug         \
        -DBUILD_SHARED_LIBS=ON         \
        -DLLVM_USE_LINKER=lld          \
        -DLLVM_ENABLE_PROJECTS="clang" \
        -DLLVM_TARGETS_TO_BUILD="Mips;Cpu0"   \
        -DLLVM_OPTIMIZED_TABLEGEN=ON   \
        ../llvm && make -j8