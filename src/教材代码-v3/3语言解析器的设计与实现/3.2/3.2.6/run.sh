#!/bin/bash
## compile
flex simple_analyzer.l
gcc lexer_main.c lex.yy.c -o SimpleFlex

## run
echo "./SimpleFlex test.sim"
./SimpleFlex test.sim
echo ""

echo "./SimpleFlex test_wrong.sim"
./SimpleFlex test_wrong.sim
echo ""

