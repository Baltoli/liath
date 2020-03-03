#include <stdio.h>
#include <stdlib.h>

float dot(float *a, float *b, int n) {
  float sum = 0.0f;
  for (int i = 0; i < n; ++i) {
    sum += a[i] * b[i];
  }
  return sum;
}

int usage(char *me) {
  printf("Usage: %s N (> 0)\n", me);
  return 1;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    return usage(argv[0]);
  }

  int length = atoi(argv[1]);
  if (length < 1) {
    return usage(argv[0]);
  }
}
