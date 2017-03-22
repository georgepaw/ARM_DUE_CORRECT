#pragma once

#include <stdint.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include "secded.hh"
#include "instruction_secded.hh"

class ASM_Function
{
public:
  ASM_Function(std::string func_name, std::string section_name, uint64_t start_address);


  void add_instruction(Instruction_SECDED instruction);
  void change_instruction(uint64_t index, Instruction_SECDED instruction);
  std::vector<Instruction_SECDED>* instructions();

  std::string func_name();
  std::string section_name();
  bool is_text();
  uint64_t start_address();
  uint64_t num_instructions();
  std::vector<uint64_t> * non_directive_instruction_offsets();
  uint64_t num_non_directive_instruction_offsets();

  std::string to_string();

private:
  std::vector<Instruction_SECDED> _instructions;
  std::string _func_name;
  std::string _section_name;
  bool _is_text;
  uint64_t _start_address;
  std::vector<uint64_t> _non_directive_instruction_offsets;
};