#include "functions.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <vector>

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
