#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>

uint32_t calc(uint32_t a, uint32_t b) {return a / b;}

int main()
{
	uint32_t a = 20;
	uint32_t b = 2;

	uint32_t c = a + b;
	uint32_t d = a - b;

	printf("%u %u %u\n", c, d, calc(a,b));

	return 0;

}