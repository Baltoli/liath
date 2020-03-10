#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int usage(char *me) {
  printf("Usage: %s N (> 0)\n", me);
  return 1;
}

float rand_float() { return (float)rand() / (float)RAND_MAX; }

struct matrix {
  float *data;
  int n;
};

float *pat(struct matrix mat, int row, int col) {
  return &mat.data[row * mat.n + col];
}

float at(struct matrix mat, int row, int col) { return *pat(mat, row, col); }

struct matrix alloc_matrix(int n) {
  struct matrix mat;

  mat.n = n;
  mat.data = malloc(sizeof(float) * n * n);

  return mat;
}

struct matrix zero_matrix(int n) {
  struct matrix mat = alloc_matrix(n);
  bzero(mat.data, mat.n * sizeof(float));
  return mat;
}

struct matrix random_matrix(int n) {
  struct matrix mat = alloc_matrix(n);

  for (int row = 0; row < mat.n; ++row) {
    for (int col = row; col < mat.n; ++col) {
      float val = rand_float();
      *pat(mat, row, col) = val;
      *pat(mat, col, row) = val;
    }
  }

  for (int i = 0; i < mat.n; ++i) {
    *pat(mat, i, i) += mat.n;
  }

  return mat;
}

struct matrix decomp(struct matrix mat) {
  struct matrix lower = zero_matrix(mat.n);

  for (int i = 0; i < mat.n; ++i) {
    for (int j = 0; j < (i + 1); ++j) {
      float sum = 0.0f;
      for (int k = 0; k < j; ++k) {
        sum += at(lower, i, k) * at(lower, j, k);
      }

      if (i == j) {
        *pat(lower, i, j) = sqrt(at(mat, i, i) - sum);
      } else {
        *pat(lower, i, j) = 1.0f / at(lower, j, j) * (at(mat, i, j) - sum);
      }
    }
  }

  return lower;
}

void show(struct matrix mat) {
  for (int row = 0; row < mat.n; ++row) {
    for (int col = 0; col < mat.n; ++col) {
      printf("%.3f ", at(mat, row, col));
    }
    printf("\n");
  }
}

void free_matrix(struct matrix mat) { free(mat.data); }

int main(int argc, char **argv) {
  if (argc < 2) {
    return usage(argv[0]);
  }

  int n = atoi(argv[1]);
  if (n < 0) {
    return usage(argv[0]);
  }

  struct matrix mat = random_matrix(n);

  show(mat);

  struct matrix low = decomp(mat);

  printf("\n");
  show(low);

  free_matrix(mat);
  free_matrix(low);
}
