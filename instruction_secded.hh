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
  SECDED secded();
  uint64_t offset();
  std::set<uint8_t> faults_locations();
  SECDED original_secded();
  uint32_t get_crc_index() { return _crc_index; }
  void set_crc_index(uint32_t crc_index) { _crc_index = crc_index; }
private:
  SECDED _secded;
  uint64_t _offset;
  std::set<uint8_t> _faults_locations;
  SECDED _original_secded;
  bool _is_directive;
  uint32_t _crc_index;

};