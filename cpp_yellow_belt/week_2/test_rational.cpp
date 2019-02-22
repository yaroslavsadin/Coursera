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
void AssertEqual(const T& t, const U& u, int line = __LINE__, 
                    string msg = {}, string file = __FILE__) {
  if (t != u) {
    ostringstream os;
    os << "Assertion failed: " << t << " != " << u << endl;
    os << "./" << file <<":" << line << "; Message: " << msg;
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

#if 1
int gcd(int a, int b) {
  if (a > 0 && b > 0) {
    if (a > b) {
      a %= b;
    } else {
      b %= a;
    }
    return gcd(a,b);
  } else {
    return (a > 0) ? a : b;
  }
}

class Rational {
public:
  // Вы можете вставлять сюда различные реализации,
  // чтобы проверить, что ваши тесты пропускают корректный код
  // и ловят некорректный

  Rational() {
    p = 0;
    q = 1;
  }
  Rational(int numerator, int denominator) {
    int gcd_value = gcd(abs(numerator),abs(denominator));
    p = numerator/gcd_value;
    q = denominator/gcd_value;
    if(q < 0) {
        if(p < 0) {
            q = abs(q);
            p = abs(p);
        } else {
            q = abs(q);
            p = -p;
        }
    }
    if(p == 0) {
        q = 1;
    }
  }

  int Numerator() const {
    return p;
  }

  int Denominator() const {
    return q;
  }
private:
  int p, q;
};
#endif
void TestRationalDefaultConstructor() {
  Rational r;
  AssertEqual(r.Numerator(),0,__LINE__,"Default constructor numerator isn't zero");
  AssertEqual(r.Denominator(),1,__LINE__,"Default constructor Denominator isn't one");
}

void TestRationalReduction() {
  {
    Rational r(4,16);
    AssertEqual(r.Numerator(),1,__LINE__,"4/16 Num");
    AssertEqual(r.Denominator(),4,__LINE__,"4/16 Denom");
  }
  {
    Rational r(6,16);
    AssertEqual(r.Numerator(),3,__LINE__,"6/16 Num");
    AssertEqual(r.Denominator(),8,__LINE__,"6/16 Denom");
  }
  {
    Rational r(20,90);
    AssertEqual(r.Numerator(),2,__LINE__,"20/100 Num");
    AssertEqual(r.Denominator(),9,__LINE__,"20/100 Denom");
  }
}

void TestRationalNegative() {
  {
    Rational r(1,-3);
    AssertEqual(r.Numerator(),-1,__LINE__,"1/(-3) Num");
    AssertEqual(r.Denominator(),3,__LINE__,"1/(-3) Denom");
  }
}

void TestRationalPositive() {
  {
    Rational r(-1,-3);
    AssertEqual(r.Numerator(),1,__LINE__,"-1/(-3) Num");
    AssertEqual(r.Denominator(),3,__LINE__,"-1/(-3) Denom");
  }
}

void TestRationalZero() {
  {
    Rational r(0,-3);
    AssertEqual(r.Numerator(),0,__LINE__,"0/(-3) Num");
    AssertEqual(r.Denominator(),1,__LINE__,"0/(-3) Denom");
  }
}

int main() {
  TestRunner runner;
  // добавьте сюда свои тесты
  runner.RunTest(TestRationalDefaultConstructor,"TestRationalDefaultConstructor");
  runner.RunTest(TestRationalReduction,"TestRationalReduction");
  runner.RunTest(TestRationalNegative,"TestRationalNegative");
  runner.RunTest(TestRationalPositive,"TestRationalPositive");
  runner.RunTest(TestRationalZero,"TestRationalZero");
  return 0;
}
