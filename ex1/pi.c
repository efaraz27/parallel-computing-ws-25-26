#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

  if (argc < 2) {
    printf("%s\n", "incorrect usage");
    return 1;
  }

  long nt = atoi(argv[1]);
  double apprx = 0.0;
  double denominator = 1.0;
  int sign = 1;

  for (long i = 0; i < nt; i++) {
    apprx += sign / denominator;
    denominator += 2;
    sign *= -1;
  }

  printf("%f\n", apprx * 4);

  return 0;
}
