#pragma once

#include <stdint.h>
#include <stdio.h>
#include <vector>
#include <set>
#include <iostream>
#include <algorithm>
#include <random>
#include "secded.hh"
#include "instruction_secded.hh"

namespace fault_injector
{
	uint32_t inject_fault(uint32_t num_bitflips, Instruction_SECDED * inst);
	uint32_t inject_faults(uint32_t instructions_to_affect, uint32_t bitflips_per_instruction, std::vector<Instruction_SECDED>* instructions);
}