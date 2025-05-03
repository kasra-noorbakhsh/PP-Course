#include "SIMDInstructions.h"
#include "printRegisters.h"

void SIMDInstructions() {
    int cpu_info[4];

    __cpuid(cpu_info, 0x00000001);
    print_cpu_info(cpu_info);

    int MMX, SSE, SSE2, SSE3, AES, SSE41, SSE42, AVX, AVX2, AVX512, SHA;

    MMX = (cpu_info[3] & (1 << 23)) != 0;
    SSE = (cpu_info[3] & (1 << 25)) != 0;
    SSE2 = (cpu_info[3] & (1 << 26)) != 0;
    SSE3 = (cpu_info[2] & (1 << 0)) != 0;
    AES = (cpu_info[2] & (1 << 25)) != 0;
    SSE41 = (cpu_info[2] & (1 << 19)) != 0;
    SSE42 = (cpu_info[2] & (1 << 20)) != 0;
    AVX = (cpu_info[2] & (1 << 28)) != 0;

    __cpuid(cpu_info, 0x00000007);
    print_cpu_info(cpu_info);

    AVX2 = (cpu_info[1] & (1 << 5)) != 0;
    AVX512 = (cpu_info[1] & (1 << 16)) != 0;
    SHA = (cpu_info[1] & (1 << 29)) != 0;

    printf("MMX:    %s\n", MMX ? "Supported" : "Not supported");
    printf("SSE:    %s\n", SSE ? "Supported" : "Not supported");
    printf("SSE2:   %s\n", SSE2 ? "Supported" : "Not supported");
    printf("SSE3:   %s\n", SSE3 ? "Supported" : "Not supported");
    printf("AES:    %s\n", AES ? "Supported" : "Not supported");
    printf("SSE4.1: %s\n", SSE41 ? "Supported" : "Not supported");
    printf("SSE4.2: %s\n", SSE42 ? "Supported" : "Not supported");
    printf("AVX:    %s\n", AVX ? "Supported" : "Not supported");
    printf("AVX2:   %s\n", AVX2 ? "Supported" : "Not supported");
    printf("AVX512: %s\n", AVX512 ? "Supported" : "Not supported");
    printf("SHA:    %s\n\n", SHA ? "Supported" : "Not supported");
}
