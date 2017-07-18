#pragma once

#include <vector>


#include "asm_function.hh"
#include "crc32c.hh"

class Program
{
public:
  std::vector<ASM_Function> functions;
  std::vector<CRC32C> crcs;
  Program(std::vector<ASM_Function>);
};