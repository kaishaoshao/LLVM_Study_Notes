#!/bin/bash
flex wc.l
gcc lex.yy.c -lfl
./a.out < text.txt

