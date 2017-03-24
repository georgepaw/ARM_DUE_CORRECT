#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <iterator>
#include "secded.hh"
#include "instruction_secded.hh"
#include "asm_function.hh"
#include "vixl/src/vixl/a64/disasm-a64.h"
#include "vixl/src/vixl/a64/instructions-a64.h"

#include <string>

namespace filter
{
  std::vector<SECDED> reduce_to_valid_codewords(SECDED in);
  std::vector<SECDED> reduce_to_valid_instructions(std::vector<ASM_Function> * functions, Instruction_SECDED * invalid_instruction, std::vector<SECDED> * valid_codewords);
  std::vector<SECDED> reduce_with_prior(std::vector<std::pair<std::string, uint64_t>> * prior_pairs,  std::vector<ASM_Function> * functions, Instruction_SECDED * invalid_instruction, std::vector<SECDED> * valid_instructions);

}