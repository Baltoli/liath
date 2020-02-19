#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <vector>

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

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cout << "Usage: " << argv[0] << " M N\n";
    return 1;
  }

  auto m = std::stoul(argv[1]);
  auto n = std::stoul(argv[2]);

  auto A = std::vector<double>(m * n);
  auto x = std::vector<double>(n);
  auto y = std::vector<double>(m);

  auto rd = std::random_device{};
  auto rand_val = [&rd]() {
    auto gen = std::mt19937(rd());
    auto dist = std::uniform_real_distribution<>(0.0, 1.0);
    return dist(gen);
  };

  std::generate(A.begin(), A.end(), rand_val);
  std::generate(x.begin(), x.end(), rand_val);
  std::generate(y.begin(), y.end(), rand_val);

  auto Ap = ptr(A.data());
  auto xp = ptr(x.data());
  auto yp = ptr(y.data());

  gemv(m, n, Ap, xp, yp);

  std::cout << "[";
  auto comma = "";
  for (auto i : y) {
    std::cout << comma << i;
    comma = ", ";
  }
  std::cout << "]\n";
}
