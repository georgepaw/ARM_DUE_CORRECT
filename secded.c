#include "secded.h"

//Do standard (72,64) SECDED 
void secded_start()
{
  printf("0x%lx\n", (unsigned long)&__executable_start);
  printf("0x%lx\n", (unsigned long)&__etext);
  const uint32_t bytes = (unsigned long)&__etext - (unsigned long)&__executable_start;

  printf("There are %"PRIu32" bytes\n", bytes);

  const uint32_t num_instructions = bytes / INSTRUCTION_LEN;
  const uint32_t odd_instructions = num_instructions & 1 ? 1 : 0;
  const uint32_t size_secded = num_instructions / 2 + odd_instructions;

  printf("There are %"PRIu32" secdeds\n", size_secded);
  __secded_data = (uint8_t*)malloc(sizeof(uint8_t) * size_secded);

  uint64_t * text_seg_ptr = (uint64_t*)((unsigned long)&__executable_start);

  for(uint32_t i = 0; i < num_instructions / 2; i++)
  {
    __SECDED(text_seg_ptr[i], __secded_data[i]);
  }
  //check if there is an odd instruction
  if(odd_instructions)
  {
    uint64_t data = *(uint64_t*)((unsigned long)&__etext - INSTRUCTION_LEN);
    __SECDED(data, __secded_data[size_secded - 1]);
  }

}

uint32_t secded_check(uint64_t * start, uint32_t num_instructions)
{
  uint32_t faults = 0;
  uint8_t res;
  for(uint32_t i = 0; i < num_instructions / 2; i++)
  {
    __SECDED_CHECK(start[i], __secded_data[i], res);
    if(res)
    {
      printf("Found fault %"PRIu32"\n", i);
      faults++;
    }
  }
  if(num_instructions & 1)
  {
    uint64_t data = *((uint32_t*)(&start[num_instructions/2]));
    __SECDED_CHECK(data, __secded_data[num_instructions/2], res);
    if(res)
    {
      printf("Found fault %"PRIu32"\n", num_instructions/2);
      faults++;
    }
  }
  return faults;
}

uint32_t secded_check_all()
{
  return secded_check((uint64_t*)((unsigned long)&__executable_start),
               ((unsigned long)&__etext - (unsigned long)&__executable_start)/INSTRUCTION_LEN);
}

void secded_finalize()
{
  free(__secded_data);
}