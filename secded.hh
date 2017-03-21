#pragma once

#include <stdint.h>

#define S1 0xc14840ffU
#define S2 0x2124ff90U
#define S3 0x6cff0808U
#define S4 0xff01a444U
#define S5 0x16f092a6U
#define S6 0x101f7161U
#define S7 0x8a820f1bU

#define C1 0x1U
#define C2 0x2U
#define C3 0x4U
#define C4 0x8U
#define C5 0x10U
#define C6 0x20U
#define C7 0x40U

#define WORD_SZIE 32
#define SECDED_SIZE 7
#define CODEWORD_SIZE 39

class SECDED
{
public:
  uint32_t instruction;
  uint8_t secded;

  SECDED();
  SECDED(uint32_t instruction);
  SECDED(uint32_t instruction, uint8_t secded);

  void generate(uint32_t instruction);
  uint8_t check();
  SECDED clone();

  bool operator==(const SECDED& other);
};