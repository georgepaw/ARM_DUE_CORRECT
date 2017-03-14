#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "secded.h"
#include "instruction_secded.h"
#include "filter.h"

uint32_t reduce_to_valid_codewords(instruction_secded * in, instruction_secded ** valid);
uint32_t reduce_to_valid_instructions(uint32_t num_valid_codewords, instruction_secded * valid_codewords, instruction_secded ** valid_instructions);
