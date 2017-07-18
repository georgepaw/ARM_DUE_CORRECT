#include "program.hh"

Program::Program(std::vector<ASM_Function> functions)
{
  CRC32C current_crc;
  uint32_t count = 0;
  uint32_t crc_index = 0;
  for(ASM_Function f : functions)
  {
    for(Instruction_SECDED &i : *(f.instructions()))
    {
      i.set_crc_index(crc_index);
      current_crc.add_instruction(i.secded().instruction);
      count++;
      if(count == INSTRUCTIONS_PER_CRC){
        count = 0;
        this->crcs.push_back(current_crc);
        current_crc.clean();
        crc_index++;
      }
    }
    this->functions.push_back(f);
  }
}