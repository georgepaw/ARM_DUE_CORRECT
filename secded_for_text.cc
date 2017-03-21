#include "secded_for_text.hh"

namespace SECDED_for_text
{
  std::vector<ASM_Function> generate_secded_for_text(const char * filename)
  {
    std::vector<ASM_Function> functions = elf_extractor::extract_functions(filename);

    vixl::Disassembler disassm;
    vixl::Decoder decoder;
    vixl::Instruction inst;

    decoder.AppendVisitor(&disassm);

    for (ASM_Function function : functions)
    {
      std::cout << "Function " << function.get_func_name() << " " << " in section " << function.get_section_name() << std::endl;
      for(Instruction_SECDED instruction: *(function.get_instructions()))
      {
        if(instruction.is_directive()) continue;
        inst.SetInstructionBits(instruction.secded.instruction);
        decoder.Decode(&inst);
        std::string disassm_out = disassm.GetOutput();
        std::cout << "0x" << std::setfill('0') << std::setw(8) << std::hex << instruction.secded.instruction << " " << disassm.GetOutput() << std::endl;
        std::string unallocated = "unallocated";
        if (disassm_out.find(unallocated) != std::string::npos) {
          std::cout << "Failed to correctly identify the instruction (unallocated)" << " "
                    << "0x" << std::setfill('0') << std::setw(8) << std::hex << instruction.secded.instruction << std::endl;
          exit(-1);
        }
        std::string unimplemented = "unimplemented";
        if (disassm_out.find(unimplemented) != std::string::npos) {
          std::cout << "Failed to correctly identify the instruction (unimplemented)" << " "
                    << "0x" << std::setfill('0') << std::setw(8) << std::hex << instruction.secded.instruction << std::endl;
          exit(-1);
        }
      }
    }

    return functions;
  }

  uint64_t check_secded(std::vector<ASM_Function>* functions)
  {
    uint64_t faulty = 0;
    for (ASM_Function &function : *functions)
    {
      for(uint64_t i = 0; i < function.get_num_instructions(); i++)
      {
        Instruction_SECDED * instruction = &(*(function.get_instructions()))[i];
        if((*instruction).secded.check())
        {
          std::cout << "Faulty Instruction " << i << " in function " << function.get_func_name() << std::endl;
          std::cout << (*instruction).to_string();
          faulty++;

          std::vector<SECDED> valid_codewords = filter::reduce_to_valid_codewords(&((*instruction).secded));
          std::cout << "There are " << std::dec << valid_codewords.size() << " valid codewords" << std::endl;

          // for(SECDED i : valid_codewords)
          // {
          //  std::cout << "Should be " << "0x" << std::setfill('0') << std::setw(2) << std::hex << (uint32_t)i.secded << " "
          //            << "0x" << std::setfill('0') << std::setw(8) << std::hex << i.instruction << std::endl;
          // }

          std::vector<SECDED> valid_instructions = filter::reduce_to_valid_instructions(instruction, &valid_codewords);
          std::cout << "There are " << std::dec << valid_instructions.size() << " valid codewords" << std::endl;

          if(valid_instructions.size() == 1)
          {
            if(valid_instructions[0] == (*instruction).original_secded)
            {
              std::cout << "Successfuly corrected instructions." << std::endl;
            }
          }
        }
      }
    }
    return faulty;
  }

  void print_text_and_secded(std::vector<ASM_Function>* functions)
  {
    for (ASM_Function function : *functions)
    {
      std::cout << function.to_string() << std::endl;
    }
  }
}