#pragma once

#include <stdint.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <map>
#include <sstream>
#include <fstream>
#include "secded.hh"
#include "instruction_secded.hh"
#include "filter.hh"
#include "elf_extractor.hh"
#include "asm_function.hh"
#include "vixl/src/vixl/a64/disasm-a64.h"
#include "vixl/src/vixl/a64/instructions-a64.h"

#define PRIOR_FILE "prior.dat"

namespace trainer
{
  void train(std::vector<std::string> * filenames);
}