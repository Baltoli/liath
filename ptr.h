#pragma once

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
