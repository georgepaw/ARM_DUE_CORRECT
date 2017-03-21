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

namespace elf_extractor
{
  std::vector<ASM_Function> extract_functions(const char * filename);
  std::string exec_function(const char* cmd);
}