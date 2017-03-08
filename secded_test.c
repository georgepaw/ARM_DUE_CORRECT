#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "secded.h"
#include <time.h>

#define SIZE 200000ULL

uint64_t rand_uint64(void) {
  uint64_t r = 0;
  for (int i=0; i<64; i += 30) {
    r = r*(RAND_MAX + (uint64_t)1) + rand();
  }
  return r;
}

int main(void)
{
	uint64_t counter = 0;
	uint64_t data[SIZE];
	uint8_t secdeds[SIZE];
	srand(time(NULL));
	for(uint64_t i = 0; i < SIZE; i++)
	{
		data[i] = rand_uint64();
	}
	for(uint64_t i = 0; i < SIZE; i++)
	{
		__SECDED(data[i], secdeds[i]);
	}
	for(uint64_t i = 0; i < SIZE; i++)
	{
		uint8_t result;
		__SECDED_CHECK(data[i], secdeds[i], result);
		if(result) printf("SECDED not working\n");
		else counter++;
	}
	printf("Correct %"PRIu64" of %"PRIu64".\n",counter, SIZE);
	counter = 0;

	for(uint32_t i = 0; i < SIZE; i++)
  {
  	uint32_t loc = rand() % 64;
  	uint8_t mem;
  	memcpy(&mem, (uint8_t*)(&data[i]) + loc/8, sizeof(uint8_t));
  	// printf("Was 0x%016"PRIX64" ", data[i]);
  	mem ^= 0x10;
  	memcpy((uint8_t*)(&data[i]) + loc/8, &mem, sizeof(uint8_t));
  	// printf("is now 0x%016"PRIX64"\n", data[i]);
  }
	for(uint64_t i = 0; i < SIZE; i++)
	{
		uint8_t result;
		__SECDED_CHECK(data[i], secdeds[i], result);
		if(result) ;//printf("SECDED not working\n");
		else counter++;
	}
	printf("Correct %"PRIu64" of %"PRIu64".\n",counter, SIZE);
  return 0;
}