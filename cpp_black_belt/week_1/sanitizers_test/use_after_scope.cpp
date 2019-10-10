#include <cstdint>
#include <functional>
#include <iostream>

using namespace std;

bool IsPrime(uint64_t x) {
  for (uint64_t i = 2; i * i <= x; ++i) {
    if (x % i == 0) {
      return false;
    }
  }
  return true;
}

int main() {
  function<uint64_t(uint64_t, uint64_t)> hash;
  for (uint64_t x = 1000000000; x <= 1000001000; ++x) {
    if (IsPrime(x)) {
      hash = [&](uint64_t lhs, uint64_t rhs) { return lhs * x + rhs; };
    }
  }
  cout << hash(1, 0) << endl;
  return 0;
}