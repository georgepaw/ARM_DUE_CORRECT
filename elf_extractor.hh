#pragma once

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <sstream>
#include <array>
#include <iterator>
#include <stdint.h>
#include "instruction_secded.hh"
#include "asm_function.hh"
#include "filter.hh"
#include "vixl/src/vixl/a64/disasm-a64.h"
#include "vixl/src/vixl/a64/instructions-a64.h"

namespace elf_extractor
{
  std::vector<ASM_Function> extract_functions(const char * filename);
}