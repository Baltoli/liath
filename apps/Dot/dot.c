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

float rand_float() { return (float)rand() / (float)RAND_MAX; }

int main(int argc, char **argv) {
  if (argc < 2) {
    return usage(argv[0]);
  }

  int length = atoi(argv[1]);
  if (length < 1) {
    return usage(argv[0]);
  }

  float *a = malloc(sizeof(*a) * length);
  float *b = malloc(sizeof(*b) * length);

  for (int i = 0; i < length; ++i) {
    a[i] = rand_float();
    b[i] = rand_float();
  }

  float ret = dot(a, b, length);
  printf("%f\n", ret);
}
