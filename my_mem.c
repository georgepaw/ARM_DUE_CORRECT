#include "my_mem.h"

uint32_t randint(uint32_t n) {
  if ((n - 1) == RAND_MAX) {
    return rand();
  } else {
    // Chop off all of the values that would cause skew...
    long end = RAND_MAX / n; // truncate skew
    end *= n;

    // ... and ignore results from rand() that fall above that limit.
    // (Worst case the loop condition should succeed 50% of the time,
    // so we can expect to bail out of this loop pretty quickly.)
    uint32_t r;
    while ((r = rand()) >= end);

    return r % n;
  }
}

uint32_t inject_bitflips()
{
	uint32_t corrupted = 0;
	uint8_t old_instruction[4] = {0x40, 0x00, 0x80, 0x52};
	uint8_t new_instruction[4] = {0x80, 0x00, 0x80, 0x52};

  uint32_t bytes = (unsigned long)&__etext - (unsigned long)&__executable_start;

  for(uint32_t byte = 0; byte < bytes - (INSTRUCTION_LEN - 1); byte++)
  {
    uint8_t match = 1;
    for(uint32_t i_byte = 0; i_byte < INSTRUCTION_LEN; i_byte++)
    {
      if(*(uint8_t*)((unsigned long)&__executable_start + byte + i_byte) != old_instruction[i_byte])
      {
        match = 0;
        break;
      }
    }
    if(match)
    {
    	corrupted++;
      memcpy((void*)((unsigned long)&__executable_start + byte), &new_instruction, sizeof(uint8_t) * INSTRUCTION_LEN);
    }
  }

  return corrupted;
}

uint32_t inject_bitflips_random(uint32_t bitflips)
{
	srand(time(NULL));
	uint32_t corrupted = 0;
  uint32_t total_bits = ((unsigned long)&__etext - (unsigned long)&__executable_start) * 8;
  for(uint32_t i = 0; i < bitflips; i++)
  {
  	corrupted++;
  	uint32_t loc = randint(total_bits);
  	uint8_t mem;
  	memcpy(&mem, (uint8_t*)((unsigned long)&__executable_start + loc/8), sizeof(uint8_t));
  	printf("At instruction %lu, was %u, ", loc/8 - (unsigned long)&__executable_start, mem);
  	mem ^= 1 << (loc % 8);
  	printf("is %u now.\n", mem);
  	memcpy((uint8_t*)((unsigned long)&__executable_start + loc/8), &mem, sizeof(uint8_t));
  }
  return corrupted;

  // __asm__ __volatile__("ic iallu\n\t" : : :"memory");
}

void print_text_segment()
{
  unsigned long bytes = (unsigned long)&__etext - (unsigned long)&__executable_start;
  for(unsigned long byte = 0; byte < bytes; byte++)
  {
    uint8_t cbyte = *(uint8_t*)((unsigned long)&__executable_start + byte);
    printf("%02X", cbyte);
  }
  printf("\n");
}

void change_text_protection()
{
  unsigned long bytes = (unsigned long)&__etext - (unsigned long)&__executable_start;
  void * p = (void *)((unsigned long)&__executable_start);
  int ret = mprotect(p, bytes, PROT_EXEC | PROT_READ | PROT_WRITE);
  if(ret)
  {
    printf("Can't change text protection\n");
    exit(1);
  }
}
