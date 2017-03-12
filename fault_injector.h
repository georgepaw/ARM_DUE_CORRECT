#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "secded.h"
#include "instruction_secded.h"
#include "gsl/gsl_rng.h"

uint32_t inject_fault(gsl_rng * r, uint32_t num_bitflips, instruction_secded * i);
uint32_t inject_faults(gsl_rng * r, uint32_t instructions_to_affect, uint32_t bitflips_per_instruction, uint64_t num_instructions, instruction_secded * instructions);
