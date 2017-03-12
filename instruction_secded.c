#include "instruction_secded.h"



instruction_secded create_instruction(uint32_t instruction, uint64_t offset)
{
	instruction_secded i;
	i.instruction = instruction;
	__SECDED(i.instruction, i.secded);
	i.offset = offset;
	i.num_faults = 0;
	i.faults_locations = NULL;
	i.original_instruction = i.instruction;
	i.original_secded = i.secded;
	return i;
}

void print_faults(instruction_secded * i)
{
	printf("Should be %02X %08X\n", (*i).original_secded, (*i).original_instruction);
	printf("Is %02X %08X\n", (*i).secded, (*i).instruction);

	if((*i).num_faults == 0)
	{
		printf("Faults (0).\n");
	}
	else
	{
		printf("Faults (%"PRIu32"): %u", (*i).num_faults, (*i).faults_locations[0]);
		for(uint32_t x = 1; x < (*i).num_faults; x++)
		{
			printf(", %u", (*i).faults_locations[x]);
		}
		printf("\n");
	}
}

void free_instruction(instruction_secded * i)
{
	if((*i).faults_locations != NULL)
	{
		free((*i).faults_locations);
	}
}

void free_instructions(uint64_t num_instructions, instruction_secded * instructions)
{
	for(uint64_t i = 0; i < num_instructions; i++)
	{
		free_instruction(&instructions[i]);
	}
}
