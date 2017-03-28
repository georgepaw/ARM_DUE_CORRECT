#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <iterator>
#include <random>
#include "secded.hh"
#include "instruction_secded.hh"
#include "asm_function.hh"
#include "vixl/src/vixl/a64/disasm-a64.h"
#include "vixl/src/vixl/a64/instructions-a64.h"
#include "vixl/src/vixl/a64/instruction-features-a64.h"

#include <string>

namespace filter
{

  enum filter_type
  {
    unsupported,
    random,
    freq,
  };

  bool valid_filter_type(std::string filter_name);
  filter_type get_filter_type(std::string filter_name);

  std::vector<SECDED> reduce_to_valid_codewords(SECDED in);
  std::vector<SECDED> reduce_to_valid_instructions(std::vector<ASM_Function> * functions, Instruction_SECDED * invalid_instruction, std::vector<SECDED> * valid_codewords);
  std::vector<SECDED> reduce_random(std::vector<SECDED> * valid_instructions);
  std::vector<SECDED> reduce_with_prior(std::vector<std::pair<std::string, uint64_t>> * prior_pairs,  std::vector<ASM_Function> * functions, Instruction_SECDED * invalid_instruction, std::vector<SECDED> * valid_instructions);
  bool check_instruction_valid(std::vector<ASM_Function> * functions, vixl::InstructionFeature * base_instruction_features, const SECDED candidate, const std::string disassm_out, vixl::Instruction * inst, Instruction_SECDED * invalid_instruction);
}