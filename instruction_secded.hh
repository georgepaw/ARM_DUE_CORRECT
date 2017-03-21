#pragma once

#include <stdint.h>
#include <vector>
#include <set>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "secded.hh"

class Instruction_SECDED
{
public:
  Instruction_SECDED(uint32_t instruction, uint64_t offset, bool is_directive);
  std::string to_string();
  uint32_t inject_fault(uint8_t location);
  bool is_directive();

//this should be private
  SECDED secded;
  uint64_t offset;
  std::set<uint8_t> faults_locations;
  SECDED original_secded;
  bool _is_directive;

};