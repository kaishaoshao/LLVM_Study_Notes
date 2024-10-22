#!/bin/bash
## compile
bison -d bison.y
flex flex.l
gcc lex.yy.c bison.tab.c main.c syntax_tree.c


## run
echo "Analyse input1.txt:"
./a.out < input1.txt
echo ""

echo "Analyse input2.txt:"
./a.out < input2.txt
echo ""
