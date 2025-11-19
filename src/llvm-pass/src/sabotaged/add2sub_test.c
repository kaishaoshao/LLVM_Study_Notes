// 受害者代码
// add2sub_test.c
#include <stdio.h>
#include <stdlib.h>

// 使用 volatile 防止编译器优化掉这个函数
int add(int a, int b) {
    return a + b; 
}

int main(int argc, char** argv) {
    // 不要直接写 5 和 3，模拟从外部读取，编译器就不敢提前计算了
    volatile int x = 5;
    volatile int y = 3;
    
    printf("Result: x + y = %d\n", add(x, y));
    return 0;
}

