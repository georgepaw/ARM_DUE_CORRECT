#include "filter.h"

uint32_t reduce_to_valid_codewords(instruction_secded * in, instruction_secded ** valid)
{
	uint32_t candidates = 0;
	const uint32_t invalid_instruction = (*in).instruction;
	const uint8_t invalid_secded = (*in).secded;

	*valid = (instruction_secded*)malloc(sizeof(instruction_secded)*100);
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
				// printf("%X %u %u\n", instruction, i, j);

			uint8_t result;
			__SECDED_CHECK(instruction, secded, result);
			if(!result)
			{
				if(__builtin_popcount(instruction ^ invalid_instruction)
					+ __builtin_popcount(secded ^ invalid_secded) != 2)
				{
					printf("Something has gone wrong here\n");
					exit(-1);
				}
				(*valid)[candidates].instruction = instruction;
				(*valid)[candidates].secded = secded;
				candidates++;
			}
		}
	}
	return candidates;
}

uint32_t reduce_to_valid_instructions(uint32_t num_valid_codewords, instruction_secded * valid_codewords, instruction_secded ** valid_instructions)
{
	uint32_t candidates = 0;

	*valid_instructions = (instruction_secded*)malloc(sizeof(instruction_secded)*num_valid_codewords);
	for(uint32_t i = 0; i < num_valid_codewords; i++)
	{
		if(1) //if instruction is valid
		{
			memcpy(&((*valid_instructions)[candidates]), &valid_codewords[i], sizeof(instruction_secded));
			candidates++;
		}
	}
	return candidates;
}
