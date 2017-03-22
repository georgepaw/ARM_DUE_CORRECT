#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <vector>
#include "secded.hh"
#include "instruction_secded.hh"
#include "vixl/src/vixl/a64/disasm-a64.h"
#include "vixl/src/vixl/a64/instructions-a64.h"

#include <string>

namespace filter
{
  std::vector<SECDED> reduce_to_valid_codewords(SECDED in);
  std::vector<SECDED> reduce_to_valid_instructions(Instruction_SECDED * invalid_instruction, std::vector<SECDED> * valid_codewords);
}