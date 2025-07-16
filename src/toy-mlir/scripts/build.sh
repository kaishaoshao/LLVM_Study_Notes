#!/bin/bash

mkdir -p build
CC=clang CXX=clang++ cmake -S . -B build && make -C build -j $(nproc)