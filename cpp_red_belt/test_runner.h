#include <sstream>
#include <exception>
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>

#pragma once

// !!!!!!!!
using namespace std;

template <class T>
ostream& operator << (ostream& os, const vector<T>& s) {
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
ostream& operator << (ostream& os, const set<T>& s) {
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
ostream& operator << (ostream& os, const map<K, V>& m) {
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

template<class T, class U>
void AssertEqual(const T& t, const U& u, string msg = {}) {
  if (t != u) {
    ostringstream os;
    os << "Assertion failed: " << t << " != " << u << endl;
    os << msg;
    throw runtime_error(os.str());
  }
}

void Assert(bool b, string msg = {});

constexpr auto RED_TEXT_START = "\033[1;31m";
constexpr auto BLUE_TEXT_START = "\033[1;36m";
constexpr auto TEXT_COLOR_RESET = "\033[0m";

class TestRunner {
public:
  template <class TestFunc>
  void RunTest(TestFunc func, const string& test_name) {
    try {
      func();
      cerr << BLUE_TEXT_START << test_name << " OK" << TEXT_COLOR_RESET << endl;
    } catch (exception& e) {
      ++fail_count;
      cerr << RED_TEXT_START << test_name << " fail: " << e.what() << TEXT_COLOR_RESET << endl;
    } catch (...) {
      ++fail_count;
      cerr << RED_TEXT_START << "Unknown exception caught" << TEXT_COLOR_RESET << endl;
    }
  }

  ~TestRunner();

private:
  int fail_count = 0;
};

#define ASSERT_EQUAL(x,y) {         \
  ostringstream ss;                 \
  ss << #x << " != " << #y <<       \
  endl << "./" << __FILE__ << ":"   \
  << __LINE__;                      \
  AssertEqual(x,y,ss.str());        \
}

#define ASSERT(x) {                 \
  ostringstream ss;                 \
  ss << #x << " is false " <<       \
  endl << "./" << __FILE__ << ":"   \
  << __LINE__;                      \
  Assert(x,ss.str());               \
}

#define RUN_TEST(tr,func)           \
  tr.RunTest(func,#func)
