#include <stdio.h>
#include <stdlib.h>

int usage(char *me) {
  printf("Usage: %s N (> 0)\n", me);
  return 1;
}

float rand_float() { return (float)rand() / (float)RAND_MAX; }

float *at(float *mat, int n, int row, int col) { return &mat[row * n + col]; }

void show(float *mat, int n) {
  for (int row = 0; row < n; ++row) {
    for (int col = 0; col < n; ++col) {
      printf("%.3f ", *at(mat, n, row, col));
    }
    printf("\n");
  }
}

int main(int argc, char **argv) {
  if (argc < 2) {
    return usage(argv[0]);
  }

  int n = atoi(argv[1]);
  if (n < 0) {
    return usage(argv[0]);
  }

  float *mat = malloc(sizeof(*mat) * n * n);

  for (int row = 0; row < n; ++row) {
    for (int col = row; col < n; ++col) {
      float val = rand_float();
      *at(mat, n, row, col) = val;
      *at(mat, n, col, row) = val;
    }
  }

  show(mat, n);

  free(mat);
}
