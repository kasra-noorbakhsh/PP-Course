#include "printRegisters.h"

void print_cpu_info(int cpu_info[4]) {
    const char* register_names[] = { "eax", "ebx", "ecx", "edx" };

    for (int i = 0; i < 4; i++) {
        printf("%s: %d\n", register_names[i], cpu_info[i]);
    }

    printf("\n");
}
