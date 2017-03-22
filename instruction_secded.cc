#include "instruction_secded.hh"

Instruction_SECDED::Instruction_SECDED(uint32_t instruction, uint64_t offset, bool is_directive)
{
  this->_secded = SECDED(instruction);
  this->_offset = offset;
  this->_original_secded = _secded.clone();
  this->_is_directive = is_directive;
}

std::string Instruction_SECDED::to_string()
{
  std::stringstream ss;
  ss << "Should be " << "0x" << std::setfill('0') << std::setw(2) << std::hex << (uint32_t)_original_secded.secded << " "
                     << "0x" << std::setfill('0') << std::setw(8) << std::hex << _original_secded.instruction << std::endl;
  ss << "Is " << "0x" << std::setfill('0') << std::setw(2) << std::hex << (uint32_t)_secded.secded << " "
              << "0x" << std::setfill('0') << std::setw(8) << std::hex << _secded.instruction << std::endl;
  if(!_faults_locations.size())
  {
    ss << "Faults (0)." << std::endl;
  }
  else
  {
    ss << "Faults (" << std::dec << _faults_locations.size() << "):";
    for(uint8_t i : _faults_locations)
    {
      ss << " " << std::dec << (uint32_t)i;
    }
    ss << std::endl;
  }
  return ss.str();
}

uint32_t Instruction_SECDED::inject_fault(uint8_t location)
{
  _faults_locations.insert(location);
  if(location < 32) _secded.instruction ^= 0x1 << location;
  else _secded.secded ^= 0x1 << (location - 32);
  return 1;
}


bool Instruction_SECDED::is_directive()
{
  return _is_directive;
}

SECDED Instruction_SECDED::secded()
{
  return _secded;
}

uint64_t Instruction_SECDED::offset()
{
  return _offset;
}

std::set<uint8_t> Instruction_SECDED::faults_locations()
{
  return _faults_locations;
}

SECDED Instruction_SECDED::original_secded()
{
  return _original_secded;
}
