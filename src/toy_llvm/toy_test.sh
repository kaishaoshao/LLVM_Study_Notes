clang toy_test/test.c -c -emit-llvm -o0 -o /tmp/test.bc
llvm-dis /tmp/test.bc
cat /tmp/test.ll
./build/bin/llc /tmp/test.bc -march=toy
