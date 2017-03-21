#include "fault_injector.hh"

namespace fault_injector
{
  uint32_t inject_fault(uint32_t num_bitflips, Instruction_SECDED * inst)
  {
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, CODEWORD_SIZE - 1);
    for(uint32_t i = 0; i < num_bitflips; i++)
    {
      uint8_t index = 0;
      //randomly flip a bit in the 39bit codeword, do not flip the same bit twice
      do
      {
        index = dist(rng);
      } while((std::find((*inst).faults_locations.begin(), (*inst).faults_locations.end(), index) != (*inst).faults_locations.end()));
      (*inst).inject_fault(index);
    }
    return num_bitflips;
  }

  uint32_t inject_faults(uint32_t instructions_to_affect, uint32_t bitflips_per_instruction, std::vector<ASM_Function>* functions)
  {
    uint32_t counter = 0;
    std::set<uint64_t> indexes;

    //only inject faults in text section and don't inject faults in directives
    uint64_t num_instruction = 0;
    for(ASM_Function function : (*functions))
    {
      if(function.get_section_name() == ".text")
      {
        num_instruction += function.get_num_non_directive_instruction_offsets();
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

      for(ASM_Function &function : (*functions))
      {
        if(function.get_section_name() != ".text") continue;

        if(index < function.get_num_non_directive_instruction_offsets())
        {
          uint64_t adjusted_index = (*(function.get_non_directive_instruction_offsets()))[index];
          std::cout << "Injecting at " << std::dec << adjusted_index << " in fuction " << function.get_func_name() << std::endl;
          counter += inject_fault(bitflips_per_instruction, &((*(function.get_instructions()))[adjusted_index]));
          break;
        }
        index -= function.get_num_non_directive_instruction_offsets();
      }
    }
    return counter;
  }
}