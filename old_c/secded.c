#include "secded.h"

//Do standard (39,32) SECDED 
void secded_start()
{
  printf("0x%lx\n", (unsigned long)&__executable_start);
  printf("0x%lx\n", (unsigned long)&__etext);
  const uint32_t bytes = (unsigned long)&__etext - (unsigned long)&__executable_start;

  printf("There are %"PRIu32" bytes\n", bytes);

  const uint32_t num_instructions = bytes / INSTRUCTION_LEN;

  printf("There are %"PRIu32" instructions\n", num_instructions);
  __secded_data = (uint8_t*)malloc(sizeof(uint8_t) * num_instructions);

  uint32_t * text_seg_ptr = (uint32_t*)((unsigned long)&__executable_start);

  for(uint32_t i = 0; i < num_instructions; i++)
  {
    __SECDED(text_seg_ptr[i], __secded_data[i]);
  }

}

uint32_t secded_check(uint32_t * start, uint32_t num_instructions)
{
  uint32_t faults = 0;
  uint8_t res;
  for(uint32_t i = 0; i < num_instructions; i++)
  {
    __SECDED_CHECK(start[i], __secded_data[i], res);
    if(res)
    {
      printf("Found fault %"PRIu32"\n", i);
      faults++;
    }
  }
  return faults;
}

uint32_t secded_check_all()
{
  return secded_check((uint32_t*)((unsigned long)&__executable_start),
               ((unsigned long)&__etext - (unsigned long)&__executable_start)/INSTRUCTION_LEN);
}

void secded_finalize()
{
  free(__secded_data);
}