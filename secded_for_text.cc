#include "secded_for_text.hh"

namespace SECDED_for_text
{
	std::vector<Instruction_SECDED> generate_secded_for_text(const char * filename)
	{
		FILE *fd;

		if ((fd = fopen(filename, "rb")) == NULL) {
		  perror("Error opening file");
		  exit(EXIT_FAILURE);
		}

		fseek(fd, 0, SEEK_END);
		uint32_t num_instructions = ftell(fd) / 4;
		rewind(fd);

		std::vector<Instruction_SECDED> instructions;

		uint32_t *text = (uint32_t*)malloc(sizeof(uint32_t) * num_instructions);

		if (fread(text, sizeof(uint32_t), num_instructions, fd) != num_instructions) {
		  fprintf(stderr, "Error reading file\n");
		  exit(EXIT_FAILURE);
		}
		fclose(fd);

		for (uint32_t i = 0; i < num_instructions; i++)
		{
			instructions.push_back(Instruction_SECDED(text[i], num_instructions*4));
		}

		free(text);

		return instructions;
	}

	uint64_t check_secded(std::vector<Instruction_SECDED>* instructions)
	{
		uint64_t faulty = 0;
		for (uint64_t i = 0; i < (*instructions).size(); i++)
		{
			if((*instructions)[i].secded.check())
			{
				std::cout << "Faulty Instruction " << i << std::endl;
				std::cout << (*instructions)[i].to_string();
				faulty++;

				std::vector<SECDED> valid_codewords = filter::reduce_to_valid_codewords(&((*instructions)[i].secded));
				std::cout << "There are " << std::dec << valid_codewords.size() << " valid codewords" << std::endl;

				// for(SECDED i : valid_codewords)
				// {
				// 	std::cout << "Should be " << "0x" << std::setfill('0') << std::setw(2) << std::hex << (uint32_t)i.secded << " "
				// 					  << "0x" << std::setfill('0') << std::setw(8) << std::hex << i.instruction << std::endl;
				// }

				std::vector<SECDED> valid_instructions = filter::reduce_to_valid_instructions(&valid_codewords);
				std::cout << "There are " << std::dec << valid_instructions.size() << " valid codewords" << std::endl;
			}
		}
		return faulty;
	}

	void print_text_and_secded(std::vector<Instruction_SECDED>* instructions)
	{
		for (Instruction_SECDED i : *instructions)
		{
			std::cout << "0x" << std::setfill('0') << std::setw(2) << std::hex << (uint32_t)i.original_secded.secded << " "
							  << "0x" << std::setfill('0') << std::setw(8) << std::hex << i.original_secded.instruction << std::endl;
		}
	}
}