#include "asm_function.hh"

ASM_Function::ASM_Function(std::string func_name, std::string section_name, uint64_t start_address)
{
  this->_func_name = func_name;
  this->_section_name = section_name;
  this->_start_address = start_address;
}


void ASM_Function::add_instruction(Instruction_SECDED instruction)
{
  _instructions.push_back(instruction);
  if(!instruction.is_directive()) _non_directive_instruction_offsets.push_back(_instructions.size()-1);
}

std::vector<Instruction_SECDED>* ASM_Function::instructions()
{
  return &_instructions;
}

std::string ASM_Function::func_name()
{
  return _func_name;
}

std::string ASM_Function::section_name()
{
  return _section_name;
}

bool ASM_Function::is_text()
{
  return _is_text;
}

uint64_t ASM_Function::start_address()
{
  return _start_address;
}

uint64_t ASM_Function::num_instructions()
{
  return _instructions.size();
}

std::vector<uint64_t> * ASM_Function::non_directive_instruction_offsets()
{
  return &_non_directive_instruction_offsets;
}

uint64_t ASM_Function::num_non_directive_instruction_offsets()
{
  return _non_directive_instruction_offsets.size();
}

std::string ASM_Function::to_string()
{
  std::stringstream ss;
  ss << "Function " << _func_name << " (" << "0x" << std::setfill('0') << std::setw(16) << std::hex << _start_address <<") in section " << _section_name << std::endl;
  for(Instruction_SECDED inst : _instructions)
  {
    ss << "0x" << std::setfill('0') << std::setw(16) << std::hex << inst.offset() << ": " 
       << "0x" << std::setfill('0') << std::setw(2) << std::hex << (uint32_t)inst.secded().secded << " "
       << "0x" << std::setfill('0') << std::setw(8) << std::hex << inst.secded().instruction << std::endl;
  }
  return ss.str();
}
