#include "cacheAndTLB.h"
#include "printRegisters.h"

void cacheAndTLB() {
    int cpu_info[4];

    // Get cache and TLB information using CPUID with EAX = 2 as said in project description
    __cpuid(cpu_info, 0x00000002);
    print_cpu_info(cpu_info);

    int cacheLineSize = cpu_info[1] & 0xFF; // Cache line size in bytes
    int ways = ((cpu_info[1] >> 12) & 0xFF) + 1; // Number of ways (associativity)
    int cacheSize = ((cpu_info[2] + 1) * 512); // Size of the cache in bytes

    printf("Cache Information:\n");
    printf("Cache Line Size: %d bytes\n", cacheLineSize);
    printf("Number of Ways (Associativity): %d\n", ways);
    printf("Cache Size: %d bytes\n", cacheSize);

    // CPUID with EAX=2 provides information about L1 data cache
    int l1DataTLBEntries = (cpu_info[3] & 0xFF); // Number of L1 data TLB entries
    printf("L1 Data TLB Entries: %d\n\n", l1DataTLBEntries);
}
