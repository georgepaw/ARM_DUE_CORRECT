#include "crc32c.hh"




// process a byte at once
uint32_t CRC32C::crc32c_software_simple(uint32_t crc, const uint8_t * data, size_t num_bytes)
{
  while (num_bytes--)
  {
    crc = (crc >> 8) ^ crc32c_table[0][(crc & 0xFF) ^ *data++];
  }
  return crc;
}

uint32_t CRC32C::generate_crc()
{
  uint32_t crc = 0xFFFFFFFF;
  for(uint32_t i = 0; i < INSTRUCTIONS_PER_CRC; i++)
  {
#if defined(SOFTWARE_CRC_SPLIT)
    SPLIT_BY_4_INNER(crc, crc, instructions[i].instruction);
    crc = crc32c_software_simple(crc, &instructions[i].secded, 1);
#else
    CRC32CW(crc, crc, instructions[i].instruction);
    CRC32CB(crc, crc, instructions[i].secded);
#endif
  }
  return crc;
}


CRC32C::CRC32C()
{
  clean();
}

void CRC32C::clean()
{
  for(uint32_t i = 0; i < INSTRUCTIONS_PER_CRC; i++)
  {
    instructions[i] = SECDED(0);
  }
  crc_checksum = generate_crc();
  num_instructions_filled = 0;
}

void CRC32C::add_instruction(uint32_t instruction)
{
  instructions[num_instructions_filled++] = SECDED(instruction);
  crc_checksum = generate_crc();
}

void CRC32C::change_instruction(uint32_t index, SECDED secded)
{
  instructions[index] = secded;
}

bool CRC32C::check()
{
  return crc_checksum == generate_crc();
}


// uint8_t CRC32C::check();
