#include <array>
#include <iostream>

template <typename T>
class proxy {
 public:
  proxy(T *b, T *p) : base_(b), ptr_(p) {}

  operator T &() {
    std::cout << "R," << base_ << "," << (ptr_ - base_) << '\n';
    return *ptr_;
  }

  proxy<T> &operator=(T const &other) {
    std::cout << "W," << base_ << "," << (ptr_ - base_) << '\n';
    *ptr_ = other;
    return *this;
  }

 private:
  T *base_;
  T *ptr_;
};

template <typename T>
class ptr {
 public:
  ptr(T *p) : base_ptr_(p) {}

  proxy<T> operator[](size_t n) { return proxy(base_ptr_, base_ptr_ + n); }

 private:
  T *base_ptr_;
};

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

int main() {
  auto A = std::array{1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
  auto x = std::array{7.0, 8.0};
  auto y = std::array{0.0, 0.0, 0.0};

  auto Ap = ptr(A.data());
  auto xp = ptr(x.data());
  auto yp = ptr(y.data());

  gemv(3, 2, Ap, xp, yp);

  std::cout << "[";
  auto comma = "";
  for (auto i : y) {
    std::cout << comma << i;
    comma = ", ";
  }
  std::cout << "]\n";
}
