#include "fault_injector.hh"

namespace fault_injector
{
  uint32_t inject_fault(uint32_t num_bitflips, Instruction_SECDED * inst, CRC32C * crc)
  {
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 38);
    std::set<uint8_t> indexes = inst->faults_locations();
    for(uint32_t i = 0; i < num_bitflips; i++)
    {
      uint8_t index = 0;
      //randomly flip a bit in the 39bit codeword, do not flip the same bit twice
      do
      {
        index = dist(rng);
      } while((std::find(indexes.begin(), indexes.end(), index) != indexes.end()));
      indexes.insert(index);
      if(index < 39) inst->inject_fault(index);
      else crc->crc_checksum ^= 0x1 << (index - 39);
    }
    return num_bitflips;
  }

  uint32_t inject_faults(uint32_t instructions_to_affect, uint32_t bitflips_per_instruction, Program* program)
  {
    uint32_t counter = 0;
    std::set<uint64_t> indexes;

    //only inject faults in text section and don't inject faults in directives
    uint64_t num_instruction = 0;
    for(ASM_Function function : program->functions)
    {
      if(function.section_name() == ".text")
      {
        num_instruction += function.num_non_directive_instruction_offsets();
      }
    }

    std::mt19937_64 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937_64::result_type> dist(0, num_instruction - 1);

    //randomly select which unique instructions to affect
    //do not affect the same instruction twice
    for(uint32_t i = 0; i < instructions_to_affect; i++)
    {
      uint64_t index = 0;
      do
      {
        index = dist(rng);
      } while((std::find(indexes.begin(), indexes.end(), index) != indexes.end()));
      indexes.insert(index);

      for(ASM_Function &function : program->functions)
      {
        if(function.section_name() != ".text") continue;

        if(index < function.num_non_directive_instruction_offsets())
        {
          uint64_t adjusted_index = (*(function.non_directive_instruction_offsets()))[index];
          std::cout << "Injecting at " << std::dec << adjusted_index << " in fuction " << function.func_name() << std::endl;
          uint32_t crc_index = (*(function.instructions()))[adjusted_index].get_crc_index();
          counter += inject_fault(bitflips_per_instruction, &((*(function.instructions()))[adjusted_index]), &program->crcs[crc_index]);
          break;
        }
        index -= function.num_non_directive_instruction_offsets();
      }
    }
    return counter;
  }
}