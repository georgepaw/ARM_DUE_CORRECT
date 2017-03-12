#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include "secded.h"

typedef struct
{
	uint32_t instruction;
	uint8_t secded;
	uint64_t offset;
	uint32_t num_faults;
	uint8_t * faults_locations;
	uint32_t original_instruction;
	uint32_t original_secded;
} instruction_secded;

instruction_secded create_instruction(uint32_t instruction, uint64_t offset);

void print_faults(instruction_secded * i);
void free_instruction(instruction_secded * i);
void free_instructions(uint64_t num_instructions, instruction_secded * instructions);
