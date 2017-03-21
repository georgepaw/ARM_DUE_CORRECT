#include "secded.hh"

SECDED::SECDED()
{

}

SECDED::SECDED(uint32_t instruction)
{
  generate(instruction);
}

SECDED::SECDED(uint32_t instruction, uint8_t secded)
{
  this->instruction = instruction;
  this->secded = secded;
}

void SECDED::generate(const uint32_t instruction)
{
  this->instruction = instruction;
  this->secded =
      __builtin_parity(S1 & instruction)
    | __builtin_parity(S2 & instruction) << 1
    | __builtin_parity(S3 & instruction) << 2
    | __builtin_parity(S4 & instruction) << 3
    | __builtin_parity(S5 & instruction) << 4
    | __builtin_parity(S6 & instruction) << 5
    | __builtin_parity(S7 & instruction) << 6;
}

uint8_t SECDED::check()
{
  return
        __builtin_parity((S1 & this->instruction) ^ (this->secded & C1))
      | __builtin_parity((S2 & this->instruction) ^ (this->secded & C2)) << 1
      | __builtin_parity((S3 & this->instruction) ^ (this->secded & C3)) << 2
      | __builtin_parity((S4 & this->instruction) ^ (this->secded & C4)) << 3
      | __builtin_parity((S5 & this->instruction) ^ (this->secded & C5)) << 4
      | __builtin_parity((S6 & this->instruction) ^ (this->secded & C6)) << 5
      | __builtin_parity((S7 & this->instruction) ^ (this->secded & C7)) << 6;
}

SECDED SECDED::clone()
{
  return SECDED(this->instruction, this->secded);
}

bool SECDED::operator==(const SECDED& other) {
  return instruction == other.instruction && secded == other.secded;
}
