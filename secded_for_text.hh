#pragma once

#include <stdint.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include "secded.hh"
#include "instruction_secded.hh"
#include "filter.hh"
#include "elf_extractor.hh"
#include "asm_function.hh"
#include "vixl/src/vixl/a64/disasm-a64.h"
#include "vixl/src/vixl/a64/instructions-a64.h"

namespace SECDED_for_text
{
  std::vector<ASM_Function> generate_secded_for_text(const char * filename);
  uint64_t check_secded(std::vector<ASM_Function>* functions);
  void print_text_and_secded(std::vector<ASM_Function>* functions);
}