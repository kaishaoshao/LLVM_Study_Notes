rm ./build -rf
mkdir -p ./build
cd ./build
cmake ..
make
set -x
clang -S -emit-llvm -Xclang -disable-O0-optnone ../add2sub_test.c -o add2sub.ll
opt -load-pass-plugin=./SabotagePass.so -passes="sabotage" add2sub.ll -S -o sabotaged.ll
lli sabotaged.ll
set +x