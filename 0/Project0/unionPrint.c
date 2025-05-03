#include "unionPrint.h"

typedef union {
    __m128i reg;

    uint8_t u8[16];
    int8_t i8[16];
    uint16_t u16[8];
    int16_t i16[8];
    uint32_t u32[4];
    int32_t i32[4];
    uint64_t u64[2];
    int64_t i64[2];
} Register128;

void unionPrint(__m128i reg, PrintType type) {
    Register128 reg128;
    reg128.reg = reg;

    switch (type) {
    case U8:
        printf("Unsigned 8-bit values: ");
        for (int i = 0; i < 16; ++i) {
            printf("%u ", reg128.u8[i]);
        }
        break;
    case I8:
        printf("Signed 8-bit values: ");
        for (int i = 0; i < 16; ++i) {
            printf("%d ", reg128.i8[i]);
        }
        break;
    case U16:
        printf("Unsigned 16-bit values: ");
        for (int i = 0; i < 8; ++i) {
            printf("%u ", reg128.u16[i]);
        }
        break;
    case I16:
        printf("Signed 16-bit values: ");
        for (int i = 0; i < 8; ++i) {
            printf("%d ", reg128.i16[i]);
        }
        break;
    case U32:
        printf("Unsigned 32-bit values: ");
        for (int i = 0; i < 4; ++i) {
            printf("%u ", reg128.u32[i]);
        }
        break;
    case I32:
        printf("Signed 32-bit values: ");
        for (int i = 0; i < 4; ++i) {
            printf("%d ", reg128.i32[i]);
        }
        break;
    case U64:
        printf("Unsigned 64-bit values: ");
        for (int i = 0; i < 2; ++i) {
            printf("%llu ", reg128.u64[i]);
        }
        break;
    case I64:
        printf("Signed 64-bit values: ");
        for (int i = 0; i < 2; ++i) {
            printf("%lld ", reg128.i64[i]);
        }
        break;
    default:
        printf("Unknown type.");
        break;
    }
    printf("\n");
}
