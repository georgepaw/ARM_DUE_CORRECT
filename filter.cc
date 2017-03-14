#include "filter.hh"

namespace filter
{
	std::vector<SECDED> reduce_to_valid_codewords(SECDED * in)
	{
		std::vector<SECDED> candidates;
		const uint32_t invalid_instruction = (*in).instruction;
		const uint8_t invalid_secded = (*in).secded;

		for(uint8_t i = 0; i < 39; i++)
		{
			uint32_t t_instruction = invalid_instruction;
			uint8_t t_secded = invalid_secded;

			if(i < 32) t_instruction ^= (0x1 << i);
			else t_secded ^= (0x1 << (i - 32));

			for(uint8_t j = i + 1; j < 39; j++)
			{
				uint32_t instruction = t_instruction;
				uint8_t secded = t_secded;
				if(j < 32) instruction ^= (0x1 << j);
				else secded ^= (0x1 << (j - 32));

				SECDED candidate = SECDED(instruction, secded);
				if(!candidate.check())
				{
					if(__builtin_popcount(instruction ^ invalid_instruction)
						+ __builtin_popcount(secded ^ invalid_secded) != 2)
					{
						printf("Something has gone wrong here\n");
						exit(-1);
					}
					candidates.push_back(candidate);
				}
			}
		}
		return candidates;
	}

	std::vector<SECDED> reduce_to_valid_instructions(std::vector<SECDED> * valid_codewords)
	{
		std::vector<SECDED> candidates;
		for(SECDED candidate : *valid_codewords)
		{
			candidates.push_back(candidate);
		}
		return candidates;
	}
}