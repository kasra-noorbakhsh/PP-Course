#ifndef	UNIONPRINT_H
#define UNIONPRINT_H

#include <stdint.h>
#include <immintrin.h> 
#include <stdio.h>

typedef enum {
    U8,  // Print 16 unsigned bytes
    I8,  // Print 16 signed bytes
    U16, // Print 8 unsigned words
    I16, // Print 8 signed words
    U32, // Print 4 unsigned double words
    I32, // Print 4 signed double words
    U64, // Print 2 unsigned quad words
    I64  // Print 2 signed quad words
} PrintType;

void unionPrint(__m128i reg, PrintType type);

#endif 
