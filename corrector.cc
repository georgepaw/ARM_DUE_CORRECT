#include <iostream>
#include "asm_function.hh"
#include "secded_for_text.hh"
#include "fault_injector.hh"

int main(int argc, char *argv[])
{

	if(argc != 2)
	{
		std::cerr << "Specifiy file name." << std::endl;
		return -1;
	}


	std::vector<ASM_Function> functions = SECDED_for_text::generate_secded_for_text(argv[1]);
	// SECDED_for_text::print_text_and_secded(&instructions);

	fault_injector::inject_faults(1, 2, &functions);
	uint64_t faulty_instructions = SECDED_for_text::check_secded(&functions);
	std::cerr << "There are " << faulty_instructions << " faulty instructions." << std::endl;

	return 0;
}