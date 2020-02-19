#pragma once

#include "ptr.h"

#include <cstddef>

template <typename T>
T dot(ptr<T> a, ptr<T> b, size_t n) {
  T sum = 0;

  for (auto i = 0u; i < n; ++i) {
    sum += a[i] * b[i];
  }

  return sum;
}

template <typename T>
void gemv(size_t M, size_t N, ptr<T> A, ptr<T> x, ptr<T> y) {
  for (auto i = 0u; i < M; ++i) {
    T sum = 0;
    for (auto j = 0u; j < N; ++j) {
      sum += A[j + i * N] * x[j];
    }
    y[i] = sum;
  }
}
