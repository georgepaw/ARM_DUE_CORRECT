#include "elf_extractor.hh"

namespace elf_extractor
{

  ASM_Function extract_function(std::istringstream * iss, std::string signature, std::string section_name)
  {
    //parse the offset and function name
    std::istringstream signature_iss(signature);
    std::vector<std::string> signature_tokens{std::istream_iterator<std::string>{signature_iss}, std::istream_iterator<std::string>{}};
    uint64_t offset = std::strtoull(signature_tokens[0].c_str(),NULL,16);
    signature_tokens[1].pop_back();
    std::string function_name = signature_tokens[1];

    ASM_Function function(function_name, section_name, offset);

    std::string line;
    while(std::getline(*iss, line) && !line.empty())
    {
      std::istringstream current_line_iss(line);
      std::vector<std::string> current_line_tokens{std::istream_iterator<std::string>{current_line_iss}, std::istream_iterator<std::string>{}};
      current_line_tokens[0].pop_back();
      uint64_t offset = std::strtoull(current_line_tokens[0].c_str(),NULL,16);
      uint32_t instruction = std::strtoull(current_line_tokens[1].c_str(),NULL,16);
      bool is_directive = current_line_tokens[2][0] == '.';

      // std::cout << line << std::endl;
      // std::cout << std::hex<< offset << " " << instruction << " " <<  is_directive << std::endl;

      function.add_instruction(Instruction_SECDED(instruction, offset, is_directive));
    }
    return function;
  }

  std::string exec_function(const char* cmd)
  {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
      if (fgets(buffer.data(), 128, pipe.get()) != NULL)
        result += buffer.data();
    }
    return result;
  }

  std::vector<ASM_Function> extract_functions(const char * filename)
  {
    //this function parses the output form objdump into functions
    std::vector<ASM_Function> functions;
    std::string command = "objdump -d -z " + std::string(filename);
    std::string functions_string = exec_function(command.c_str());
    std::istringstream iss(functions_string);

    std::string line;
    std::string section_name;
    while(std::getline(iss, line))
    {
      std::istringstream current_line_iss(line);
      std::vector<std::string> current_line_tokens{std::istream_iterator<std::string>{current_line_iss}, std::istream_iterator<std::string>{}};
      //for each section
      if (line.find("Disassembly of section") != std::string::npos)
      {
        current_line_tokens[3].pop_back();
        section_name = current_line_tokens[3];

        //for each function in section:
        //skip an empty line
        std::getline(iss, line);
        //get line with the offset and function name
        std::getline(iss, line);
        functions.push_back(extract_function(&iss, line, section_name));
      }
      else if(current_line_tokens.size() == 2)
      {
        functions.push_back(extract_function(&iss, line, section_name));
      }
    }

    //check all instructions are valid
    vixl::Disassembler disassm;
    vixl::Decoder decoder;
    vixl::Instruction inst;

    decoder.AppendVisitor(&disassm);

    for (ASM_Function function : functions)
    {
      for(Instruction_SECDED instruction: *(function.instructions()))
      {
        if(instruction.is_directive()) continue;
        disassm.MapCodeAddress(instruction.offset(), &inst);
        inst.SetInstructionBits(instruction.secded().instruction);
        vixl::InstructionFeature * instruction_features = decoder.decode_and_get_features(&inst);
        std::string disassm_out = disassm.GetOutput();
        if(function.section_name() == ".text")
        {
          //use the filter to check that all instructions come out as valid
          bool valid_instruction = filter::check_instruction_valid(false, &functions, instruction_features, instruction.secded(), disassm_out, &inst, NULL);
          if (!valid_instruction) {
            std::cerr << "The filter has failed to identify this instruction as valid " << " "
                      << "0x" << std::setfill('0') << std::setw(8) << std::hex << instruction.secded().instruction << " "
                      << "VIXL: " << disassm_out << std::endl;
            exit(-1);
          }
        }
        delete instruction_features;
      }
    }

    return functions;
  }
}