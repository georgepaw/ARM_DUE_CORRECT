#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "my_mem.h"
#include "secded.h"

int main(void)
{
  change_text_protection();

  secded_start();

  // printf("Corrupted %"PRIu32" instructions. \n", inject_bitflips());
  printf("Corrupted %"PRIu32" instructions. \n", inject_bitflips_random(5));
  // print_text_segment();

  printf("There were %"PRIu32" faulty SECDEDs.\n", secded_check_all());

  //actual program
  int a = 2;
  int b = 2;
  int c = a + b;
  printf("\n\nc is %d\n", c);

  secded_finalize();
  return 0;
}
