#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>

int main()
{
	uint32_t a = 20;
	uint32_t b = 2;

	uint32_t c = a + b;
	uint32_t d = a - b;
	uint32_t e = a / b;

	printf("%u %u %u\n", c, d, e);

	return 0;

}