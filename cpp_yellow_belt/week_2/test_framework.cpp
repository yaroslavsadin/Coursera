#include <sstream>
#include <exception>
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>

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
void AssertEqual(const T& t, const U& u, int line = __LINE__, string msg = {}) {
  if (t != u) {
    ostringstream os;
    os << "Assertion failed: " << t << " != " << u << endl;
    os << "Line: " << line << "; Message: " << msg;
    throw runtime_error(os.str());
  }
}

void Assert(bool b, int line = __LINE__, string msg = {}) {
  AssertEqual(b, true, line, msg);
}

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

  ~TestRunner() {
    if (fail_count > 0) {
      cerr << RED_TEXT_START << fail_count << " unit tests failed. Terminate" << TEXT_COLOR_RESET << endl;
      exit(1);
    }
  }

private:
  int fail_count = 0;
};
