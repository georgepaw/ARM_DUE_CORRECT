#pragma once

#include <stdint.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include "secded.hh"
#include "instruction_secded.hh"
#include "filter.hh"

namespace SECDED_for_text
{
	std::vector<Instruction_SECDED> generate_secded_for_text(const char * filename);
	uint64_t check_secded(std::vector<Instruction_SECDED>* instructions);
	void print_text_and_secded(std::vector<Instruction_SECDED>* instructions);
}