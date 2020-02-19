#include <array>
#include <iostream>

template <typename T>
class proxy {
 public:
  proxy(T &e) : ref_(e) {}

  operator T &() {
    std::cout << "Read\n";
    return ref_;
  }

  proxy<T> &operator=(T const &other) {
    std::cout << "Write\n";
    ref_ = other;
    return *this;
  }

 private:
  T &ref_;
};

template <typename T>
class ptr {
 public:
  ptr(T *p) : base_ptr_(p) {}

  proxy<T> operator[](size_t n) { return proxy(base_ptr_[n]); }

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

int main() {
  auto a = std::array{1.0, 2.0, 3.0};
  auto ap = ptr(a.data());

  auto b = std::array{0.0, 1.0, 2.0};
  auto bp = ptr(b.data());

  dot(ap, bp, 3);
}
