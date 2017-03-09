#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "secded.h"
#include "my_mem.h"
#include <time.h>

#define SIZE 2000000

int main(void)
{
	uint32_t counter = 0;
	uint32_t data[SIZE];
	uint8_t secdeds[SIZE];

	srand(time(NULL));

	for(uint32_t i = 0; i < SIZE; i++)
	{
		data[i] = rand();
	}
	for(uint32_t i = 0; i < SIZE; i++)
	{
		__SECDED(data[i], secdeds[i]);
	}
	for(uint32_t i = 0; i < SIZE; i++)
	{
		uint8_t result;
		__SECDED_CHECK(data[i], secdeds[i], result);
		if(result) printf("SECDED not working\n");
		else counter++;
	}
	printf("Correct %u of %u.\n",counter, SIZE);
	counter = 0;

	for(uint32_t i = 0; i < SIZE; i++)
  {
  	// printf("Was 0x%08"PRIX32" ", data[i]);
  	for(uint32_t j = 0; j < 32; j++)
  	{
  		data[i] ^= (rand() & 1) << j;
  	}
  	// printf("is now 0x%08"PRIX32"\n", data[i]);
  }
	for(uint32_t i = 0; i < SIZE; i++)
	{
		uint8_t result;
		__SECDED_CHECK(data[i], secdeds[i], result);
		if(!result) counter++;
	}
	printf("Correct %u of %u.\n",counter, SIZE);
  return 0;
}