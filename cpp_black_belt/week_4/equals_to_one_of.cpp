#include "test_runner.h"

template<typename T>
bool EqualsToOneOf(const T& single) {
  return false; 
}

template<typename T1, typename T2>
bool EqualsToOneOf(const T1& first, const T2& second) {
  return first == second;
}

template<typename T1, typename T2, typename ... Ts>
bool EqualsToOneOf(const T1& first, const T2& second, const Ts& ... args) {
  return (first == second) || EqualsToOneOf(first, args...);  
}

// EqualsToOneOf(x, "apple", "orange") означает (x == "apple" || x == "orange")

void Test() {
  auto x = "pear";
  ASSERT(EqualsToOneOf(x, "pear"));
  ASSERT(!EqualsToOneOf(x, "apple"));
  ASSERT(EqualsToOneOf(x, "apple", "pear"));
  ASSERT(!EqualsToOneOf(x, "apple", "banana"));
  ASSERT(EqualsToOneOf(x, "apple", "banana", "pear"));
  ASSERT(!EqualsToOneOf(x, "apple", "banana", "peach"));
  ASSERT(EqualsToOneOf(x, "apple", "banana", "pear", "orange"));
  ASSERT(!EqualsToOneOf(x, "apple", "banana", "peach", "orange"));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, Test);
  return 0;
}
