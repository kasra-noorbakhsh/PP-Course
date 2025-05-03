#include "processorName.h"
#include "printRegisters.h"

void processorName() {
    int cpu_info[4] = { 0 };
    char brand[49];

    // 1. Get Processor Brand (Full Name)
    __cpuid(cpu_info, 0x80000002);
    print_cpu_info(cpu_info);

    memcpy(brand, cpu_info, sizeof(cpu_info));

    __cpuid(cpu_info, 0x80000003);
    print_cpu_info(cpu_info);

    memcpy(brand + 16, cpu_info, sizeof(cpu_info));

    __cpuid(cpu_info, 0x80000004);
    print_cpu_info(cpu_info);

    memcpy(brand + 32, cpu_info, sizeof(cpu_info));

    brand[48] = '\0';
    printf("Processor Name: %s\n\n", brand);

    // 2. Get Processor Family, Model, Stepping
    __cpuid(cpu_info, 1);
    print_cpu_info(cpu_info);

    int stepping = cpu_info[0] & 0xF;
    int model = (cpu_info[0] >> 4) & 0xF;
    int family = (cpu_info[0] >> 8) & 0xF;
    int extended_model = (cpu_info[0] >> 16) & 0xF;
    int extended_family = (cpu_info[0] >> 20) & 0xFF;

    if (family == 6 || family == 15) {
        model += (extended_model << 4);
    }

    if (family == 15) {
        family += extended_family;
    }

    printf("CPU Family: %d\n", family);
    printf("CPU Model: %d\n", model);
    printf("CPU Stepping: %d\n\n", stepping);

    // 3. Check if CPUID 0x16 is supported (base frequency)
    __cpuid(cpu_info, 0);
    int highest_supported_leaf = cpu_info[0];

    if (highest_supported_leaf >= 0x16) {
        __cpuid(cpu_info, 0x16);
        print_cpu_info(cpu_info);

        int base_freq = cpu_info[0]; // CPU base frequency in MHz
        if (base_freq != 0) {
            printf("Base Frequency: %d MHz\n\n", base_freq);
        }
        else {
            printf("Base Frequency: Not available\n\n");
        }
    }
    else {
        printf("Base Frequency: Not supported by this CPU\n\n");
    }
}
