#include "maxFrequency.h"
#include "printRegisters.h"

void maxFrequency() {
    int cpu_info[4] = { 0 };

    // Get maximum frequency information using CPUID with EAX = 0x16
    __cpuid(cpu_info, 0x16);
    print_cpu_info(cpu_info);

    unsigned int maxTurboFrequency = cpu_info[1] & 0x3FFF;

    printf("Maximum Working Frequency (Turbo Boost): %u MHz\n\n", maxTurboFrequency);
}
