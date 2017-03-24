#include "filter.hh"

namespace filter
{
  std::vector<SECDED> reduce_to_valid_codewords(SECDED in)
  {
    std::vector<SECDED> candidates;
    const uint32_t invalid_instruction = in.instruction;
    const uint8_t invalid_secded = in.secded;

    for(uint8_t i = 0; i < 39; i++)
    {
      uint32_t t_instruction = invalid_instruction;
      uint8_t t_secded = invalid_secded;

      if(i < 32) t_instruction ^= (0x1 << i);
      else t_secded ^= (0x1 << (i - 32));

      for(uint8_t j = i + 1; j < 39; j++)
      {
        uint32_t instruction = t_instruction;
        uint8_t secded = t_secded;
        if(j < 32) instruction ^= (0x1 << j);
        else secded ^= (0x1 << (j - 32));

        SECDED candidate = SECDED(instruction, secded);
        if(!candidate.check())
        {
          if(__builtin_popcount(instruction ^ invalid_instruction)
            + __builtin_popcount(secded ^ invalid_secded) != 2)
          {
            printf("Something has gone wrong here\n");
            exit(-1);
          }
          candidates.push_back(candidate);
        }
      }
    }
    return candidates;
  }

  std::vector<SECDED> reduce_to_valid_instructions(std::vector<ASM_Function> * functions, Instruction_SECDED * invalid_instruction, std::vector<SECDED> * valid_codewords)
  {
    std::vector<SECDED> candidates;
    vixl::Disassembler disassm;
    vixl::Decoder decoder;
    vixl::Instruction inst;

    decoder.AppendVisitor(&disassm);

    for(SECDED candidate : *valid_codewords)
    {

      disassm.MapCodeAddress((*invalid_instruction).offset(), &inst);
      inst.SetInstructionBits(candidate.instruction);
      decoder.Decode(&inst);
      std::string disassm_out = disassm.GetOutput();
      if (disassm_out.find("unallocated") != std::string::npos)
      {
        //this case should never happen
        if((*invalid_instruction).original_secded().instruction == candidate.instruction)
        {
          std::cout << "Failed to correctly identify the instruction" << std::endl;
          std::cout << "instruction: " << "0x" << std::setfill('0') << std::setw(2) << std::hex << (uint32_t)candidate.secded << " "
                    << "0x" << std::setfill('0') << std::setw(8) << std::hex << candidate.instruction << std::endl;
          exit(-1);
        }
        continue;
      }
      if (disassm_out.find("unimplemented") != std::string::npos)
      {
        //this case should never happen
        if((*invalid_instruction).original_secded().instruction == candidate.instruction)
        {
          std::cout << "Failed to correctly identify the instruction" << std::endl;
          std::cout << "instruction: " << "0x" << std::setfill('0') << std::setw(2) << std::hex << (uint32_t)candidate.secded << " "
                    << "0x" << std::setfill('0') << std::setw(8) << std::hex << candidate.instruction << std::endl;
          exit(-1);
        }
        continue;
      }

      bool valid_instruction = true;
      //further analysis
      //if it is a branch instruction, check it branches to a valid address
      if (disassm_out.find("(addr") != std::string::npos && inst.BranchType() != vixl::UnknownBranchType)
      {
        std::istringstream disass_iss(disassm_out);
        std::vector<std::string> current_line_tokens{std::istream_iterator<std::string>{disass_iss}, std::istream_iterator<std::string>{}};
        std::stringstream ss;

        current_line_tokens.back().pop_back();
        ss << std::hex << current_line_tokens.back();
        uint64_t address;
        ss >> address;
        //check if it is an address to any instruction
        valid_instruction = false;
        for(ASM_Function function : *functions)
        {
          for(Instruction_SECDED i : *function.instructions())
          {
            if(i.offset() == address)
            {
              valid_instruction = true;
              break;
            }
          }
        }
      }

      std::cout << "instruction: " << "0x" << std::setfill('0') << std::setw(2) << std::hex << (uint32_t)candidate.secded << " "
                << "0x" << std::setfill('0') << std::setw(8) << std::hex << candidate.instruction << " "
                << "VIXL: " << disassm_out << " "
                << "valid? " << valid_instruction << std::endl;

      if(!valid_instruction && (*invalid_instruction).original_secded().instruction == candidate.instruction)
      {
        std::cout << "Instruction incorrectly identified as invalid!" << std::endl;
        exit(-1);
      }

      if(valid_instruction) candidates.push_back(candidate);
    }
    return candidates;
  }

  std::vector<SECDED> reduce_with_prior(std::vector<std::pair<std::string, uint64_t>> * prior_pairs,  std::vector<ASM_Function> * functions, Instruction_SECDED * invalid_instruction, std::vector<SECDED> * valid_instructions)
  {
    std::vector<SECDED> candidates;
    vixl::Disassembler disassm;
    vixl::Decoder decoder;
    vixl::Instruction inst;

    decoder.AppendVisitor(&disassm);


    //sort with smallest instruction first
    std::sort((*valid_instructions).begin(), (*valid_instructions).end(), [](const SECDED& a, const SECDED& b)
    {
      return a.instruction < b.instruction;
    });

    std::vector<std::string> candidates_mnemonics;
    for(SECDED candidate : *valid_instructions)
    {
      disassm.MapCodeAddress((*invalid_instruction).offset(), &inst);
      inst.SetInstructionBits(candidate.instruction);
      decoder.Decode(&inst);
      std::istringstream disass_iss(disassm.GetOutput());
      std::vector<std::string> current_line_tokens{std::istream_iterator<std::string>{disass_iss}, std::istream_iterator<std::string>{}};
      candidates_mnemonics.push_back(current_line_tokens[0]);
    }

    for(std::pair<std::string, uint64_t> p : *prior_pairs)
    {
      bool match_found = false;
      for(uint32_t i = 0 ; i < (*valid_instructions).size(); i++)
      {
        if(candidates_mnemonics[i] != p.first) continue;
        match_found = true;
        candidates.push_back((*valid_instructions)[i]);


        disassm.MapCodeAddress((*invalid_instruction).offset(), &inst);
        inst.SetInstructionBits((*valid_instructions)[i].instruction);
        decoder.Decode(&inst);
        std::cout << "Assuming " << disassm.GetOutput() << " is the correct instruction." << std::endl;
        break;
      }
      if(match_found) break;
    }

    return candidates;
  }
}
