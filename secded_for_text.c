#include "secded_for_text.h"

uint32_t generate_secded_for_text(const char * filename, instruction_secded ** instructions)
{
	FILE *fd;

	if ((fd = fopen(filename, "rb")) == NULL) {
	  perror("Error opening file");
	  exit(EXIT_FAILURE);
	}

	fseek(fd, 0, SEEK_END);
	uint32_t num_instructions = ftell(fd) / 4;
	rewind(fd);

	*instructions = (instruction_secded*)malloc(sizeof(instruction_secded) * num_instructions);
	uint32_t *text = (uint32_t*)malloc(sizeof(uint32_t) * num_instructions);

	if (fread(text, sizeof(uint32_t), num_instructions, fd) != num_instructions) {
	  fprintf(stderr, "Error reading file\n");
	  exit(EXIT_FAILURE);
	}
	fclose(fd);

	for (uint32_t i = 0; i < num_instructions; i++)
	{
		(*instructions)[i] = create_instruction(text[i], num_instructions*4);
	}

	free(text);
	return num_instructions;
}

uint32_t check_secded(uint64_t num_instructions, instruction_secded * instructions)
{
	uint64_t faulty = 0;
	for (uint64_t i = 0; i < num_instructions; i++)
	{
		uint8_t res;
		__SECDED_CHECK(instructions[i].instruction, instructions[i].secded, res);
		if(res)
		{
			printf("Faulty instruction %"PRIu64"\n", i);
			print_faults(&instructions[i]);
			faulty++;

			instruction_secded * valid_codewords = NULL;
			uint32_t num_valid_codewords = reduce_to_valid_codewords(&instructions[i], &valid_codewords);
			printf("There are %"PRIu32" valid codewords\n", num_valid_codewords);
			print_text_and_secded(num_valid_codewords, valid_codewords);

			free_instructions(num_valid_codewords, valid_codewords);
			free(valid_codewords);
		}
	}
	return faulty;
}

void print_text_and_secded(uint64_t num_instructions, instruction_secded * instructions)
{
	for (uint64_t i = 0; i < num_instructions; i++)
	{
	  printf("%02X %08X\n", instructions[i].secded, instructions[i].instruction);
	}
}
