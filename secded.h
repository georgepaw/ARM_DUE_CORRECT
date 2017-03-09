#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include "my_mem.h"

// #define S1 0x0111111630f0f0ffULL
// #define S2 0x02222226cf00ff0fULL
// #define S3 0x64444440f0ff0f0cULL
// #define S4 0x68888880ff0f00f3ULL
// #define S5 0xcf00f0ff01111116ULL
// #define S6 0x30f0ff0f02222226ULL
// #define S7 0xf0ff00f364444440ULL
// #define S8 0xff0f0f0c68888880ULL

#define S1 0xc14840ffU
#define S2 0x2124ff90U
#define S3 0x6cff0808U
#define S4 0xff01a444U
#define S5 0x16f092a6U
#define S6 0x101f7161U
#define S7 0x8a820f1bU

#define C1 0x1U
#define C2 0x2U
#define C3 0x4U
#define C4 0x8U
#define C5 0x10U
#define C6 0x20U
#define C7 0x40U

#define __SECDED(in, out)        	\
  if(1) {                        	\
    const uint8_t bits[] =       	\
    {                            	\
      __builtin_parity(S1 & in), 	\
      __builtin_parity(S2 & in), 	\
      __builtin_parity(S3 & in), 	\
      __builtin_parity(S4 & in), 	\
      __builtin_parity(S5 & in), 	\
      __builtin_parity(S6 & in), 	\
      __builtin_parity(S7 & in), 	\
    };                           	\
    out = bits[0]                	\
        | bits[1] << 1           	\
        | bits[2] << 2           	\
        | bits[3] << 3           	\
        | bits[4] << 4           	\
        | bits[5] << 5           	\
        | bits[6] << 6;          	\
  } else

#define __SECDED_CHECK(data_in, secded_in, out)                   \
  if(1) {                                                         \
    out =                                                         \
        __builtin_parity((S1 & data_in) ^ (secded_in & C1))       \
      | __builtin_parity((S2 & data_in) ^ (secded_in & C2)) << 1	\
      | __builtin_parity((S3 & data_in) ^ (secded_in & C3)) << 2	\
      | __builtin_parity((S4 & data_in) ^ (secded_in & C4)) << 3	\
      | __builtin_parity((S5 & data_in) ^ (secded_in & C5)) << 4	\
      | __builtin_parity((S6 & data_in) ^ (secded_in & C6)) << 5	\
      | __builtin_parity((S7 & data_in) ^ (secded_in & C7)) << 6; \
  } else

uint8_t * __secded_data;

void secded_start();
uint32_t secded_check(uint32_t * start, uint32_t num_instructions);
uint32_t secded_check_all();
void secded_finalize();