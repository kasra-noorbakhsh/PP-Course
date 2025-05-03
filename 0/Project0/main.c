//this project is developed by Kasra Noorbakhsh (SID: 810100230) and Shahnam Feizian (SID: 810100197)

#include "processorName.h"
#include "processorCores.h"
#include "hyperThreadingCheck.h"
#include "turboBoost.h"
#include "maxFrequency.h"
#include "SIMDInstructions.h"
#include "cacheAndTLB.h"
#include "unionPrint.h"

int main() {
	processorName();
	processorCores();
	hyperThreadingCheck();
	turboBoost();
	maxFrequency();
	SIMDInstructions();
	cacheAndTLB();

    __m128i reg = _mm_setr_epi8(127, -128, 255, 0, 1, -1, 100, -100, 50, -50, 200, -200, 10, -10, 30, -30);

    printf("Printing as 16 Unsigned Bytes:\n");
    unionPrint(reg, U8);

    printf("Printing as 16 Signed Bytes:\n");
    unionPrint(reg, I8);

    printf("Printing as 8 Unsigned Words:\n");
    unionPrint(reg, U16);

    printf("Printing as 8 Signed Words:\n");
    unionPrint(reg, I16);

    printf("Printing as 4 Unsigned Double Words:\n");
    unionPrint(reg, U32);

    printf("Printing as 4 Signed Double Words:\n");
    unionPrint(reg, I32);

    printf("Printing as 2 Unsigned Quad Words:\n");
    unionPrint(reg, U64);

    printf("Printing as 2 Signed Quad Words:\n");
    unionPrint(reg, I64);
	return 0;
}
