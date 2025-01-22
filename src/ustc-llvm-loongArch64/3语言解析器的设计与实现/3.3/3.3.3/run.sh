#!/bin/bash
## compile
bison -d calc.y
flex calc.l
gcc lex.yy.c calc.tab.c driver.c -o calc

## run
./calc < text.txt

