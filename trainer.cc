#include "trainer.hh"

namespace trainer
{
  void train(std::vector<std::string> * filenames)
  {
    std::vector<std::vector<ASM_Function>> files;
    for(std::string filename : *filenames)
    {
      files.push_back(elf_extractor::extract_functions(filename.c_str()));
    }
    std::map<std::string, uint64_t> instruction_freq;

    vixl::Disassembler disassm;
    vixl::Decoder decoder;
    vixl::Instruction inst;

    decoder.AppendVisitor(&disassm);

    for(std::vector<ASM_Function> functions : files)
    {
      for(ASM_Function function : functions)
      {
        for(Instruction_SECDED i : *function.instructions())
        {
          if(i.is_directive()) continue;
          disassm.MapCodeAddress(i.offset(), &inst);
          inst.SetInstructionBits(i.secded().instruction);
          decoder.Decode(&inst);
          std::istringstream disass_iss(disassm.GetOutput());
          std::vector<std::string> tokens{std::istream_iterator<std::string>{disass_iss}, std::istream_iterator<std::string>{}};

          if (instruction_freq.find(tokens[0]) == instruction_freq.end())
          {
            instruction_freq[tokens[0]] = 0;
          }
          instruction_freq[tokens[0]]++;
        }
      }
    }

    std::vector<std::pair<std::string, uint64_t>> pairs;
    for (auto itr = instruction_freq.begin(); itr != instruction_freq.end(); ++itr)
      pairs.push_back(*itr);

    std::sort(pairs.begin(), pairs.end(), [](const std::pair<std::string, uint64_t>& a, const std::pair<std::string, uint64_t>& b)
    {
      return a.second > b.second;
    });

    std::ofstream output_file;
    output_file.open (PRIOR_FILE);

    for(std::pair<std::string, uint64_t> p : pairs)
    {
      output_file << p.first << " " << p.second << std::endl;
    }
    output_file.close();
  }

}