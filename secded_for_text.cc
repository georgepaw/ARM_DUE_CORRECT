#include "secded_for_text.hh"

namespace SECDED_for_text
{
  std::vector<ASM_Function> generate_secded_for_text(const char * filename)
  {
    std::vector<ASM_Function> functions = elf_extractor::extract_functions(filename);

    // vixl::Disassembler disassm;
    // vixl::Decoder decoder;
    // vixl::Instruction inst;

    // decoder.AppendVisitor(&disassm);

    // for (ASM_Function function : functions)
    // {
    //   for(Instruction_SECDED instruction: *(function.instructions()))
    //   {
    //     if(instruction.is_directive()) continue;
    //     inst.SetInstructionBits(instruction.secded().instruction);
    //     decoder.Decode(&inst);
    //     std::string disassm_out = disassm.GetOutput();
    //     // std::cout << "0x" << std::setfill('0') << std::setw(8) << std::hex << instruction.secded().instruction << " " << disassm.GetOutput() << std::endl;
    //     std::string unallocated = "unallocated";
    //     if (disassm_out.find(unallocated) != std::string::npos) {
    //       std::cout << "Failed to correctly identify the instruction (unallocated)" << " "
    //                 << "0x" << std::setfill('0') << std::setw(8) << std::hex << instruction.secded().instruction << std::endl;
    //       exit(-1);
    //     }
    //     std::string unimplemented = "unimplemented";
    //     if (disassm_out.find(unimplemented) != std::string::npos) {
    //       std::cout << "Failed to correctly identify the instruction (unimplemented)" << " "
    //                 << "0x" << std::setfill('0') << std::setw(8) << std::hex << instruction.secded().instruction << std::endl;
    //       exit(-1);
    //     }
    //   }
    // }

    return functions;
  }

  uint64_t check_secded(std::vector<ASM_Function>* functions)
  {
    //read prior in
    std::ifstream prior_file(PRIOR_FILE);
    std::vector<std::pair<std::string, uint64_t>> pairs;
    if (prior_file.is_open())
    {
      std::string line;
      while(getline(prior_file, line))
      {
        std::istringstream iss(line);
        std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};
        std::pair<std::string, uint64_t> p;
        std::stringstream ss;

        p.first = tokens[0];
        ss << tokens[1];
        ss >> p.second;

        pairs.push_back(p);

      }
      prior_file.close();
    }

    std::cout << "Prior loaded " << pairs.size() << std::endl;

    uint64_t faulty = 0;
    for (ASM_Function &function : *functions)
    {
      for(uint64_t i = 0; i < function.num_instructions(); i++)
      {
        Instruction_SECDED * instruction = &(*(function.instructions()))[i];
        if((*instruction).secded().check())
        {
          std::cout << "Faulty Instruction " << i << " in function " << function.func_name() << std::endl;
          std::cout << (*instruction).to_string();
          faulty++;

          std::vector<SECDED> valid_codewords = filter::reduce_to_valid_codewords((*instruction).secded());
          std::cout << "There are " << std::dec << valid_codewords.size() << " valid codewords" << std::endl;

          // for(SECDED i : valid_codewords)
          // {
          //  std::cout << "Should be " << "0x" << std::setfill('0') << std::setw(2) << std::hex << (uint32_t)i.secded << " "
          //            << "0x" << std::setfill('0') << std::setw(8) << std::hex << i.instruction << std::endl;
          // }

          std::vector<SECDED> valid_instructions = filter::reduce_to_valid_instructions(functions, instruction, &valid_codewords);
          std::cout << "There are " << std::dec << valid_instructions.size() << " valid codewords" << std::endl;

          std::vector<SECDED> reduced_instructions = filter::reduce_with_prior(&pairs, functions, instruction, &valid_instructions);
          std::cout << "There are " << std::dec << reduced_instructions.size() << " reduced_instructions" << std::endl;

          if(reduced_instructions.size() == 1)
          {
            if(reduced_instructions[0] == (*instruction).original_secded())
            {
              std::cout << "Successfuly corrected instruction." << std::endl;
            }
            else
            {
              std::cout << "Unsuccessfuly corrected instruction." << std::endl;
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