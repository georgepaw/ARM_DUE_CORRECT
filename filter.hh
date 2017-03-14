#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <vector>
#include "secded.hh"
#include "instruction_secded.hh"

namespace filter
{
	std::vector<SECDED> reduce_to_valid_codewords(SECDED * in);
	std::vector<SECDED> reduce_to_valid_instructions(std::vector<SECDED> * valid_codewords);
}