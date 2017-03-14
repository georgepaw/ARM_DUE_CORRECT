#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "secded.h"
#include "instruction_secded.h"
#include "filter.h"

uint32_t generate_secded_for_text(const char * filename, instruction_secded ** instructions);
uint32_t check_secded(uint64_t num_instructions, instruction_secded * instructions);
void print_text_and_secded(uint64_t num_instructions, instruction_secded * instructions);