#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "secded_for_text.h"
#include "instruction_secded.h"
#include "fault_injector.h"
#include "gsl/gsl_rng.h"


int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("Specifiy file name.\n");
		return -1;
	}

	instruction_secded * instructions;

	const uint64_t num_instructions = generate_secded_for_text(argv[1], &instructions);
	// print_text_and_secded(num_instructions, instructions);

	if(num_instructions > (uint64_t)((uint32_t)-1))
	{
		printf("Too many instructions, PRNG won't work.\n");
		exit(-1);
	}

	gsl_rng * r;
	gsl_rng_env_setup();
  r = gsl_rng_alloc(gsl_rng_ranlxd1);
  gsl_rng_set(r, time(NULL));
  printf("Range from %lu to %lu\n", gsl_rng_min(r), gsl_rng_max(r));


	inject_faults(r, 1, 2, num_instructions, instructions);
  check_secded(num_instructions, instructions);
	print_text_and_secded(num_instructions, instructions);

	free_instructions(num_instructions, instructions);
	free(instructions);
  gsl_rng_free(r);
  return 0;
}