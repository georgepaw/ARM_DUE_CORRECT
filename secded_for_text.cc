#include "secded_for_text.hh"

namespace SECDED_for_text
{
  std::vector<ASM_Function> generate_secded_for_text(const char * filename)
  {
    std::vector<ASM_Function> functions = elf_extractor::extract_functions(filename, true);

    return functions;
  }

  uint64_t check_secded(Program * program, filter::filter_type ft)
  {
    uint64_t faulty = 0;
    for (ASM_Function &function : program->functions)
    {
      for(uint64_t i = 0; i < function.num_instructions(); i++)
      {
        Instruction_SECDED * instruction = &(*(function.instructions()))[i];
        if(instruction->secded().check())
        {
          std::cout << "Faulty Instruction " << i << " in function " << function.func_name() << std::endl;
          std::cout << instruction->to_string();
          faulty++;

          std::vector<SECDED> valid_codewords = filter::reduce_to_valid_codewords(instruction->offset(), instruction->get_crc_index(), program);
          std::cout << "There are " << std::dec << valid_codewords.size() << " valid codewords" << std::endl;

          // for(SECDED i : valid_codewords)
          // {
          //  std::cout << "Should be " << "0x" << std::setfill('0') << std::setw(2) << std::hex << (uint32_t)i.secded << " "
          //            << "0x" << std::setfill('0') << std::setw(8) << std::hex << i.instruction << std::endl;
          // }

          std::vector<SECDED> valid_instructions = filter::reduce_to_valid_instructions(&(program->functions), instruction, &valid_codewords);
          std::cout << "There are " << std::dec << valid_instructions.size() << " valid codewords" << std::endl;

          std::vector<SECDED> reduced_instructions;

          switch(ft)
          {
            case filter::filter_type::random:
            {
              reduced_instructions = filter::reduce_random(&valid_instructions);
              break;
            }
            case filter::filter_type::freq:
            {
              std::vector<std::pair<std::string, uint64_t>> pairs;
              //read prior in
              std::ifstream prior_file(PRIOR_FILE);
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
              reduced_instructions = filter::reduce_with_prior(&pairs, &(program->functions), instruction, &valid_instructions);
              break;
            }
            default:
              std::cerr << "This filter type is not supported at the moment" << std::endl;
              exit(-1);
              break;
          }
          std::cout << "There are " << std::dec << reduced_instructions.size() << " reduced_instructions" << std::endl;

          if(reduced_instructions.size() == 1)
          {
            if(reduced_instructions[0] == instruction->original_secded())
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