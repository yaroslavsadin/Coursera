#pragma once

#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace TestRunnerPrivate {
  template <typename K, typename V, template <typename, typename> class Map>
  std::ostream& PrintMap(std::ostream& os, const Map<K, V>& m) {
    os << "{";
    bool first = true;
    for (const auto& kv : m) {
      if (!first) {
        os << ", ";
      }
      first = false;
      os << kv.first << ": " << kv.second;
    }
    return os << "}";
  }
}

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& s) {
  os << "{";
  bool first = true;
  for (const auto& x : s) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << x;
  }
  return os << "}";
}

template <class T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& s) {
  os << "{";
  bool first = true;
  for (const auto& x : s) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << x;
  }
  return os << "}";
}

template <class K, class V>
std::ostream& operator<<(std::ostream& os, const std::map<K, V>& m) {
  return TestRunnerPrivate::PrintMap(os, m);
}

template <class K, class V>
std::ostream& operator<<(std::ostream& os, const std::unordered_map<K, V>& m) {
  return TestRunnerPrivate::PrintMap(os, m);
}

template <class T, class U>
void AssertEqual(const T& t, const U& u, const std::string& hint = {}) {
  if (!(t == u)) {
    std::ostringstream os;
    os << "Assertion failed: " << t << " != " << u;
    if (!hint.empty()) {
      os << " hint: " << hint;
    }
    throw std::runtime_error(os.str());
  }
}

inline void Assert(bool b, const std::string& hint) {
  AssertEqual(b, true, hint);
}

constexpr auto RED_TEXT_START = "\033[1;31m";
constexpr auto CYAN_TEXT_START = "\033[1;36m";
constexpr auto BLUE_TEXT_START = "\033[1;34m";
constexpr auto MAGENTA_TEXT_START = "\033[1;35m";
constexpr auto YELLOW_TEXT_START = "\033[1;33m";
constexpr auto TEXT_COLOR_RESET = "\033[0m";

class TestRunner {
public:
  TestRunner() {
    std::cerr << BLUE_TEXT_START << "============== Test Runner Start ==============" << TEXT_COLOR_RESET << std::endl;
  }
  template <class TestFunc>
  void RunTest(TestFunc func, const std::string& test_name) {
    try {
      func();
      std::cerr << CYAN_TEXT_START << test_name << " OK" << TEXT_COLOR_RESET << std::endl;
    } catch (std::exception& e) {
      ++fail_count;
      std::cerr << RED_TEXT_START << test_name << " fail: " << std::endl << TEXT_COLOR_RESET << e.what() << std::endl;
    } catch (...) {
      ++fail_count;
      std::cerr << RED_TEXT_START << "Unknown exception caught" << TEXT_COLOR_RESET << std::endl;
    }
  }

  ~TestRunner() {
    std::cerr << BLUE_TEXT_START << "============= Test Runner Finished ============" << TEXT_COLOR_RESET << std::endl;
    std::cerr.flush();
    if (fail_count > 0) {
      std::cerr << fail_count << " unit tests failed. Terminate" << std::endl;
      exit(1);
    }
  }

private:
  int fail_count = 0;
};

#ifndef FILE_NAME
#define FILE_NAME __FILE__
#endif

#define ASSERT_EQUAL(x, y)                                               \
  {                                                                      \
    std::ostringstream __assert_equal_private_os;                        \
    __assert_equal_private_os << #x << " != " << #y << "\n" << FILE_NAME \
                              << ":" << __LINE__;                        \
    AssertEqual(x, y, __assert_equal_private_os.str());                  \
  }

#define ASSERT(x)                                                  \
  {                                                                \
    std::ostringstream __assert_private_os;                        \
    __assert_private_os << #x << " is false\n" << FILE_NAME << ":" \
                        << __LINE__;                               \
    Assert(x, __assert_private_os.str());                          \
  }

#define RUN_TEST(tr, func) tr.RunTest(func, #func)
