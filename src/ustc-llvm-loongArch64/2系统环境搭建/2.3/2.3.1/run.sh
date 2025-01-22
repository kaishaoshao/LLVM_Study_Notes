#!/bin/bash
clang -S -emit-llvm gcd.c
lli gcd.ll
echo $?
