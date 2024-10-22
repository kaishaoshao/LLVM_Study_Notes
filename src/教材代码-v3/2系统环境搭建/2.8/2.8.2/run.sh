#!/bin/bash
## compile
loongarch64-unknown-linux-gnu-gcc -static hello-world.S -o hello-world

## run
qemu-loongarch64 ./hello-world
