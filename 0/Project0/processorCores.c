#include "processorCores.h"
#include "printRegisters.h"

void processorCores() {
    int cpu_info[4] = { 0 };
    int cores_per_package = 0;

    // CPUID with EAX = 4, ECX = 0 (determine the number of cores)
    __cpuidex(cpu_info, 4, 0);
    print_cpu_info(cpu_info);

    int cores = ((cpu_info[0] >> 26) & 0x3F) + 1; // Bits 26:31 in EAX give number of cores - 1

    printf("Physical Cores: %d\n\n", cores);

    // Get the number of logical cores using CPUID with EAX = 1
    __cpuid(cpu_info, 1);
    print_cpu_info(cpu_info);

    int logical_cores = (cpu_info[1] >> 16) & 0xFF;  // Bits 16-23 of EBX give the number of logical processors

    printf("Logical Cores: %d\n\n", logical_cores);
}
