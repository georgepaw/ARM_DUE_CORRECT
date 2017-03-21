#include "asm_function.hh"

ASM_Function::ASM_Function(std::string func_name, std::string section_name, uint64_t start_address)
{
	this->func_name = func_name;
	this->section_name = section_name;
	this->start_address = start_address;
}


void ASM_Function::add_instruction(Instruction_SECDED instruction)
{
	instructions.push_back(instruction);
	if(!instruction.is_directive()) non_directive_instruction_offsets.push_back(instructions.size()-1);
}

std::vector<Instruction_SECDED>* ASM_Function::get_instructions()
{
	return &instructions;
}

std::string ASM_Function::get_func_name()
{
	return func_name;
}

std::string ASM_Function::get_section_name()
{
	return section_name;
}

bool ASM_Function::is_text()
{
	return _is_text;
}

uint64_t ASM_Function::get_start_address()
{
	return start_address;
}

uint64_t ASM_Function::get_num_instructions()
{
	return instructions.size();
}

std::vector<uint64_t> * ASM_Function::get_non_directive_instruction_offsets()
{
	return &non_directive_instruction_offsets;
}

uint64_t ASM_Function::get_num_non_directive_instruction_offsets()
{
	return non_directive_instruction_offsets.size();
}

std::string ASM_Function::to_string()
{
	return "";
}
