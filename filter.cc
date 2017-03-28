#include "filter.hh"

namespace filter
{
  bool valid_filter_type(std::string filter_name)
  {
    if(filter_name == "random") return true;
    else if(filter_name == "freq") return true;
    return false;
  }

  filter_type get_filter_type(std::string filter_name)
  {
    if(filter_name == "random") return filter_type::random;
    else if(filter_name == "freq") return filter_type::freq;
    return filter_type::unsupported;
  }


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
            std::cerr << "Something has gone wrong here." << std::endl;
            exit(-1);
          }
          candidates.push_back(candidate);
        }
      }
    }
    return candidates;
  }

  bool check_instruction_valid(std::vector<ASM_Function> * functions, vixl::InstructionFeature * base_instruction_features, const SECDED candidate, const std::string disassm_out, vixl::Instruction * inst, Instruction_SECDED * invalid_instruction)
  {
    bool valid_instruction = true;
    switch((*base_instruction_features).get_base_type())
    {
      case vixl::InstructionType::Unallocated:
      {
        return false;
        break;
      }
      case vixl::InstructionType::Unimplemented:
      {
        return false;
        break;
      }
      case vixl::InstructionType::PCRelAddressing:
      {
        vixl::PCRelAddressingInstruction * instruction_features
          = static_cast<vixl::PCRelAddressingInstruction*>(base_instruction_features);
        break;
      }
      case vixl::InstructionType::AddSubImmediate:
      {
        vixl::AddSubImmediateInstruction * instruction_features
          = static_cast<vixl::AddSubImmediateInstruction*>(base_instruction_features);

        if((*instruction_features).get_type() == vixl::AddSubImmediateType::Unallocated) return false;
        break;
      }
      case vixl::InstructionType::DataProcessing:
      {
        vixl::DataProcessingInstruction * instruction_features
          = static_cast<vixl::DataProcessingInstruction*>(base_instruction_features);

        if((*instruction_features).get_type() == vixl::DataProcessingType::Unallocated) return false;
        break;
      }
      case vixl::InstructionType::Logical:
      {
        vixl::LogicalInstruction * instruction_features
          = static_cast<vixl::LogicalInstruction*>(base_instruction_features);

        if((*instruction_features).get_type() == vixl::LogicalType::Unallocated) return false;
        break;
      }
      case vixl::InstructionType::BitfieldExtract:
      {
        vixl::BitfieldExtractInstruction * instruction_features
          = static_cast<vixl::BitfieldExtractInstruction*>(base_instruction_features);

        if((*instruction_features).get_type() == vixl::BitfieldExtractType::Unallocated) return false;
        break;
      }
      case vixl::InstructionType::BranchSystemException:
      {
        vixl::BranchSystemExceptionInstruction * instruction_features
          = static_cast<vixl::BranchSystemExceptionInstruction*>(base_instruction_features);

        if((*instruction_features).get_type() == vixl::BranchSystemExceptionType::Unallocated) return false;

        if (disassm_out.find("(addr") != std::string::npos && (*inst).BranchType() != vixl::UnknownBranchType)
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
          for(uint32_t i = 0; i < (*functions).size(); i++)
          {

            if(i < (*functions).size() - 1
              && !((*functions)[i].start_address() <= address && address < (*functions)[i + 1].start_address()))
                continue;

            ASM_Function function = (*functions)[i];
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
        break;
      }
      case vixl::InstructionType::LoadStore:
      {
        vixl::LoadStoreInstruction * instruction_features
          = static_cast<vixl::LoadStoreInstruction*>(base_instruction_features);

        if((*instruction_features).get_type() == vixl::LoadStoreType::Unallocated
        || (*instruction_features).get_type() == vixl::LoadStoreType::Unimplemented) return false;
        break;
      }
      case vixl::InstructionType::FP:
      {
        vixl::FPInstruction * instruction_features
          = static_cast<vixl::FPInstruction*>(base_instruction_features);

        if((*instruction_features).get_type() == vixl::FPType::Unallocated) return false;
        break;
      }
    }
    return valid_instruction;
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
      vixl::InstructionFeature * instruction_features = decoder.decode_and_get_features(&inst);

      // if(instruction_features == NULL) std::cout << "Poop\n";

      //   switch((*instruction_features).get_base_type())
      //   {
      //   case vixl::InstructionType::Unallocated:
      //     std::cout << "Unallocated" << std::endl;
      //     break;
      //   case vixl::InstructionType::Unimplemented:
      //     std::cout << "Unimplemented" << std::endl;
      //     break;
      //   case vixl::InstructionType::PCRelAddressing:
      //     std::cout << "PCRelAddressing" << std::endl;
      //     break;
      //   case vixl::InstructionType::AddSubImmediate:
      //     std::cout << "AddSubImmediate" << std::endl;
      //     break;
      //   case vixl::InstructionType::DataProcessing:
      //     std::cout << "DataProcessing" << std::endl;
      //     break;
      //   case vixl::InstructionType::Logical:
      //     std::cout << "Logical" << std::endl;
      //     break;
      //   case vixl::InstructionType::BitfieldExtract:
      //     std::cout << "BitfieldExtract" << std::endl;
      //     break;
      //   case vixl::InstructionType::BranchSystemException:
      //     std::cout << "BranchSystemException" << std::endl;
      //     break;
      //   case vixl::InstructionType::LoadStore:
      //     std::cout << "LoadStore" << std::endl;
      //     break;
      //   case vixl::InstructionType::FP:
      //     std::cout << "FP" << std::endl;
      //     break;
      //   }


      std::string disassm_out = disassm.GetOutput();

      bool valid_instruction = check_instruction_valid(functions, instruction_features, candidate, disassm_out, &inst, invalid_instruction);

      // valid_instruction = instruction_filter(functions, candidate, disassm_out, &inst, invalid_instruction);

      std::cout << "instruction: " << "0x" << std::setfill('0') << std::setw(2) << std::hex << (uint32_t)candidate.secded << " "
                << "0x" << std::setfill('0') << std::setw(8) << std::hex << candidate.instruction << " "
                << "VIXL: " << disassm_out << " "
                << "valid? " << valid_instruction << std::endl;

      if(!valid_instruction && (*invalid_instruction).original_secded().instruction == candidate.instruction)
      {
        std::cerr << "Instruction incorrectly identified as invalid!" << std::endl;
      }

      if(valid_instruction) candidates.push_back(candidate);
    }
    return candidates;
  }

  std::vector<SECDED> reduce_random(std::vector<SECDED> * valid_instructions)
  {
    std::vector<SECDED> candidates;
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, (*valid_instructions).size() - 1);

    candidates.push_back((*valid_instructions)[dist(rng)]);
    std::cout << "Assuming " << "0x" << std::setfill('0') << std::setw(8) << std::hex << candidates[0].instruction << std::endl;
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
