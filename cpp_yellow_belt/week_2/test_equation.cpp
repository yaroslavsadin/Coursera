#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
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

int GetLinearRootsCount(int b,int c) {
  if (b == 0) {
    return 0;
  } else {
    return 1;
  }
}

#if 1
int GetDistinctRealRootCount(double a, double b, double c) {
  // Вы можете вставлять сюда различные реализации функции,
  // чтобы проверить, что ваши тесты пропускают корректный код
  // и ловят некорректный
  if (a == 0) {
    return GetLinearRootsCount(b,c);
  }
    
  int D = b*b - 4*a*c;
  if (D > 0) {
    return 2;
  } else if(D == 0) {
    return 1;
  } else {
    return 0;
  }
}
#endif

void TestZeroRoots(void) {
  {
    int expected = 0;
    int result = GetDistinctRealRootCount(1, 0, 1);
    AssertEqual(expected,result,__LINE__, "D less than zero");
  }
  {
    int expected = 0;
    int result = GetDistinctRealRootCount(5, 3, 6);
    AssertEqual(expected,result,__LINE__, "D less than zero");
  }
  {
    int expected = 0;
    int result = GetDistinctRealRootCount(0, 0, 1);
    AssertEqual(expected,result,__LINE__, "Not an equation");
  }
}
void TestOneRoot(void) {
  {
    int expected = 1;
    int result = GetDistinctRealRootCount(0, 1, 1);
    AssertEqual(expected,result,__LINE__, "Linear equation");
  }
  {
    int expected = 1;
    int result = GetDistinctRealRootCount(1, 2, 1);
    AssertEqual(expected,result,__LINE__, "D equals zero");
  }
  {
    int expected = 1;
    int result = GetDistinctRealRootCount(0,5,7);
    AssertEqual(expected,result,__LINE__, "Linear equation");
  }
  {
    int expected = 1;
    int result = GetDistinctRealRootCount(4, 4, 1);
    AssertEqual(expected,result,__LINE__, "D equals zero");
  }
}
void TestTwoRoots(void) {
    {
      int expected = 2;
      int result = GetDistinctRealRootCount(1, 4, 2);
      AssertEqual(expected,result,__LINE__, "D more than zero");
    }
    {
      int expected = 2;
      int result = GetDistinctRealRootCount(1, 2, 0);
      AssertEqual(expected,result,__LINE__, "D more than zero");
    }
    {
      int expected = 2;
      int result = GetDistinctRealRootCount(1, 3, 2);
      AssertEqual(expected,result,__LINE__, "D more than zero");
    }
    
}

int main() {
  TestRunner runner;
  // добавьте сюда свои тесты
  runner.RunTest(TestZeroRoots,"TestZeroRoots");
  runner.RunTest(TestOneRoot,"TestOneRoot");
  runner.RunTest(TestTwoRoots,"TestTwoRoots");
  return 0;
}
