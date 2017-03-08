#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include "my_mem.h"

#define S1 0x0111111630f0f0ffULL
#define S2 0x02222226cf00ff0fULL
#define S3 0x64444440f0ff0f0cULL
#define S4 0x68888880ff0f00f3ULL
#define S5 0xcf00f0ff01111116ULL
#define S6 0x30f0ff0f02222226ULL
#define S7 0xf0ff00f364444440ULL
#define S8 0xff0f0f0c68888880ULL

#define C1 0x1
#define C2 0x2
#define C3 0x4
#define C4 0x8
#define C5 0x10
#define C6 0x20
#define C7 0x40
#define C8 0x80

#define __SECDED(in, out)        		\
  if(1) {                        		\
    const uint8_t bits[] =       		\
    {                            		\
      __builtin_parityll(S1 & in), 	\
      __builtin_parityll(S2 & in), 	\
      __builtin_parityll(S3 & in), 	\
      __builtin_parityll(S4 & in), 	\
      __builtin_parityll(S5 & in), 	\
      __builtin_parityll(S6 & in), 	\
      __builtin_parityll(S7 & in), 	\
      __builtin_parityll(S8 & in)  	\
    };                           		\
    out = bits[0]                		\
        | bits[1] << 1           		\
        | bits[2] << 2           		\
        | bits[3] << 3           		\
        | bits[4] << 4           		\
        | bits[5] << 5           		\
        | bits[6] << 6           		\
        | bits[7] << 7;          		\
  } else

#define __SECDED_CHECK(data_in, secded_in, out)               	\
  if(1) {                                                     	\
    out =                                                     	\
        __builtin_parityll((S1 & data_in) ^ (secded_in & C1))		\
      + __builtin_parityll((S2 & data_in) ^ (secded_in & C2)) 	\
      + __builtin_parityll((S3 & data_in) ^ (secded_in & C3)) 	\
      + __builtin_parityll((S4 & data_in) ^ (secded_in & C4)) 	\
      + __builtin_parityll((S5 & data_in) ^ (secded_in & C5)) 	\
      + __builtin_parityll((S6 & data_in) ^ (secded_in & C6)) 	\
      + __builtin_parityll((S7 & data_in) ^ (secded_in & C7)) 	\
      + __builtin_parityll((S8 & data_in) ^ (secded_in & C8));	\
  } else

uint8_t * __secded_data;

void secded_start();
uint32_t secded_check(uint64_t * start, uint32_t num_instructions);
uint32_t secded_check_all();
void secded_finalize();