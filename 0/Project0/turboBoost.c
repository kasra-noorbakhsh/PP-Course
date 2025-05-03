#include "turboBoost.h"	
#include "printRegisters.h"

void turboBoost() {
    int cpu_info[4] = { 0 };

    // Get Turbo Boost information using CPUID with EAX = 0x1F
    __cpuid(cpu_info, 0x1F);
    print_cpu_info(cpu_info);

    // Check Bit 0 of EBX to determine support for Turbo Boost Max Technology 3.0
    int turboBoostMaxSupported = cpu_info[1] & 0x1;

    if (turboBoostMaxSupported) {
        printf("Turbo Boost Max Technology 3.0 is supported.\n\n");
    }
    else {
        printf("Turbo Boost Max Technology 3.0 is not supported.\n\n");
    }
}
