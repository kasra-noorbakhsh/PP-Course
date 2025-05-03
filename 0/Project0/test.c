#include <stdio.h>

int main() {
    int info[4];

    asm volatile (
    "mov $16, %%eax; cpuid;"
    : "=a"(info[0]), "=b"(info[1]), "=c"(info[2]), "=d"(info[3]):
    );

    printf("%d\n", info[0]);
    printf("%d\n", info[1]);
    printf("%d\n", info[2]);
    printf("%d\n", info[3]);
}
