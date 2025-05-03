#include "hyperThreadingCheck.h"
#include "printRegisters.h"

void hyperThreadingCheck() {
    int cpu_info[4] = { 0 };

    // CPUID with EAX = 1 to check Hyper-Threading support in EDX (bit 28)
    __cpuid(cpu_info, 1);
    print_cpu_info(cpu_info);

    int hyperthreading_supported = (cpu_info[3] >> 28) & 0x1;

    if (hyperthreading_supported) {
        printf("Hyper-Threading is supported.\n\n");
    }
    else {
        printf("Hyper-Threading is not supported.\n\n");
    }
}
