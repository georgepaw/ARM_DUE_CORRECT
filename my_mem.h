#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>

#define START_ADDRESS 0x401910
#define END_ADDRESS 0x40193c

#define INSTRUCTION_LEN 4

#ifdef OSX
char __executable_start;
char __etext;
#else
extern char __executable_start;
extern char __etext;
#endif

uint32_t inject_bitflips();
void print_text_segment();
void change_text_protection();
uint32_t inject_bitflips_random(uint32_t bitflips);
