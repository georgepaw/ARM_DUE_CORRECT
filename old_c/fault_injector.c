#include "fault_injector.h"

uint64_t random_uint64_t(gsl_rng * r)
{
	//for now only return 32bits of randomness
	return gsl_rng_get(r);
}

uint32_t inject_fault(gsl_rng * r, uint32_t num_bitflips, instruction_secded * inst)
{
	(*inst).faults_locations = (uint8_t*)malloc(sizeof(uint8_t) * num_bitflips);
	for(uint32_t i = 0; i < num_bitflips; i++)
	{
		uint8_t index = 0;
		uint8_t found = 0;
		//randomly flip a bit in the 39bit codeword, do not flip the same bit twice
		do
		{
			found = 1;
			index = gsl_rng_uniform_int(r, CODEWORD_SIZE);
			for(uint32_t j = 0; j < i; j++)
			{
				if(index == (*inst).faults_locations[j])
				{
					found = 0;
					break;
				}
			}
		} while(!found);
		if(index < 32) (*inst).instruction ^= (0x1 << index);
		else (*inst).secded ^= (0x1 << (index-32));
		(*inst).faults_locations[i] = index;
	}
	(*inst).num_faults = num_bitflips;
	return num_bitflips;
}
uint32_t inject_faults(gsl_rng * r, uint32_t instructions_to_affect, uint32_t bitflips_per_instruction, uint64_t num_instructions, instruction_secded * instructions)
{
	uint32_t counter = 0;
	uint64_t * indexes = (uint64_t*)malloc(sizeof(uint64_t) * instructions_to_affect);
	//randomly select which unique instructions to affect
	//do not affect the same instruction twice
	for(uint32_t i = 0; i < instructions_to_affect; i++)
	{
		uint64_t index = 0;
		uint8_t found = 0;
		do
		{
			found = 1;
			index = gsl_rng_uniform_int(r, num_instructions);
			for(uint32_t j = 0; j < i; j++)
			{
				if(index == indexes[j])
				{
					found = 0;
				}
			}
		} while(!found);
		indexes[i] = index;
		printf("Injecting at %"PRIu64"\n", index);
 		counter += inject_fault(r, bitflips_per_instruction, &instructions[index]);
	}
	free(indexes);
	return counter;
}