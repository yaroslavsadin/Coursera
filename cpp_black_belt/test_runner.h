#pragma once

#include <sstream>
#include <stdexcept>
#include <iostream>
#include <map>
#include <unordered_map>
#include <set>
#include <string>
#include <vector>
#include "ostream_color.h"

namespace TestRunnerPrivate {
  template <
    class Map
  >
  std::ostream& PrintMap(std::ostream& os, const Map& m) {
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
std::ostream& operator << (std::ostream& os, const std::vector<T>& s) {
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
std::ostream& operator << (std::ostream& os, const std::set<T>& s) {
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
std::ostream& operator << (std::ostream& os, const std::map<K, V>& m) {
  return TestRunnerPrivate::PrintMap(os, m);
}

template <class K, class V>
std::ostream& operator << (std::ostream& os, const std::unordered_map<K, V>& m) {
  return TestRunnerPrivate::PrintMap(os, m);
}

template<class T, class U>
void AssertEqual(const T& t, const U& u, const std::string& hint = {}) {
  if (!(t == u)) {
    std::ostringstream os;
    os << "Assertion failed: " << t << " != " << u << std::endl;
    if (!hint.empty()) {
       os << "Hint: " << hint;
    }
    throw std::runtime_error(os.str());
  }
}

inline void Assert(bool b, const std::string& hint) {
  AssertEqual(b, true, hint);
}

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

#define ASSERT_EQUAL(x, y) {                          \
  std::ostringstream __assert_equal_private_os;       \
  __assert_equal_private_os                           \
    << #x << " != " << #y << std::endl                \
    << FILE_NAME << ":" << __LINE__;                  \
  AssertEqual(x, y, __assert_equal_private_os.str()); \
}

#define ASSERT(x) {                           \
  std::ostringstream __assert_private_os;     \
  __assert_private_os << #x << " is false "   \
  << std::endl  << FILE_NAME << ":" << __LINE__;          \
  Assert(static_cast<bool>(x), __assert_private_os.str());       \
}

#define RUN_TEST(tr, func) \
  tr.RunTest(func, #func)

#define ASSERT_THROWS(expr, expected_exception) {                                           \
  bool __assert_private_flag = true;                                                        \
  try {                                                                                     \
    expr;                                                                                   \
    __assert_private_flag = false;                                                          \
  } catch (expected_exception&) {                                                           \
  } catch (...) {                                                                           \
    std::ostringstream __assert_private_os;                                                 \
    __assert_private_os << "Expression " #expr " threw an unexpected exception"             \
    << std::endl <<  " " FILE_NAME ":" << __LINE__;                                         \
    Assert(false, __assert_private_os.str());                                               \
  }                                                                                         \
  if (!__assert_private_flag){                                                              \
    std::ostringstream __assert_private_os;                                                 \
    __assert_private_os << "Expression " #expr " is expected to throw " #expected_exception \
    << std::endl <<  " " FILE_NAME ":" << __LINE__;                                         \
    Assert(false, __assert_private_os.str());                                               \
  }                                                                                         \
}

#define ASSERT_DOESNT_THROW(expr)                                           \
  try {                                                                     \
    expr;                                                                   \
  } catch (...) {                                                           \
    std::ostringstream __assert_private_os;                                 \
    __assert_private_os << "Expression " #expr " threw an unexpected exception" \
      " " FILE_NAME ":" << __LINE__;                                        \
    Assert(false, __assert_private_os.str());                               \
  }
