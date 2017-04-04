#include "trainer.hh"

namespace trainer
{
  void train(std::vector<std::string> * filenames)
  {
    std::vector<std::vector<ASM_Function>> files;

    uint32_t num_threads = omp_get_max_threads();
    std::cout << num_threads << std::endl;

    std::map<std::string, uint64_t> instruction_freq;
    std::vector<std::map<std::string, uint64_t>> instruction_freq_threads(num_threads);
  #pragma omp parallel
  {
    uint32_t thread_num = omp_get_thread_num();
    vixl::Disassembler disassm;
    vixl::Decoder decoder;
    vixl::Instruction inst;

    decoder.AppendVisitor(&disassm);

    #pragma omp for schedule(dynamic)
    for(uint32_t i = 0; i < filenames->size(); i++)
    {
      std::string filename = (*filenames)[i];
      std::cout << "Extracting file (" << thread_num << ") " << filename << std::endl;
      std::vector<ASM_Function> functions = elf_extractor::extract_functions(filename.c_str(), false);
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

          if (instruction_freq_threads[thread_num].find(tokens[0]) == instruction_freq_threads[thread_num].end())
          {
            instruction_freq_threads[thread_num][tokens[0]] = 0;
          }
          instruction_freq_threads[thread_num][tokens[0]]++;
        }
      }
    }
  #pragma omp critical
  {
    for (auto itr = instruction_freq_threads[thread_num].begin(); itr != instruction_freq_threads[thread_num].end(); ++itr)
    {
      if (instruction_freq_threads[thread_num].find(itr->first) == instruction_freq_threads[thread_num].end())
      {
        instruction_freq[itr->first] = itr->second;
      }
      else
      {
        instruction_freq[itr->first] += itr->second;
      }
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