#!/bin/bash

# opt工具来进行一个优化————>指令合并
opt -S -instcombine testfile.ll -o ../output/output.ll

# 使用opt工具进行无用参数消除（dead-argument-elimination）优化：
opt -S -deadargelim testfile.ll -o ../output/output2.ll
