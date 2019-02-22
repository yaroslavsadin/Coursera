#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

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


bool IsPalindrom(const string& s) {
  // Вы можете вставлять сюда различные реализации функции,
  // чтобы проверить, что ваши тесты пропускают корректный код
  // и ловят некорректный
// переберём все символы s[i] из первой половины строки
  for (size_t i = 0; i < s.size() / 3 ; ++i) {
    // s[s.size() - i - 1] — символ, симметричный символу s[i]
    if (s[i] != s[s.size() - i - 1]) {
      return false;  // если они не совпадают, строка точно не палиндром
    }
  }
  
  // если все пары символов совпадают, строка — палиндром
  return true;
}

void TestPalindrom(void) {
    Assert(IsPalindrom(""),__LINE__,"empty string");
    Assert(IsPalindrom("m"),__LINE__,"single 'm'");
    Assert(IsPalindrom("madam"),__LINE__,"madam");
    Assert(!IsPalindrom("madam "),__LINE__,"madam with tailing space");
    Assert(!IsPalindrom("gentleman"),__LINE__,"gentleman");
    Assert(!IsPalindrom(" poop"),__LINE__,"poop with leading space");
    Assert(IsPalindrom("poop"),__LINE__,"poop");
    Assert(IsPalindrom("mmm"),__LINE__,"mmm");
    Assert(IsPalindrom(" "),__LINE__,"space");
    Assert(!IsPalindrom("maaaa"),__LINE__,"maaaa");
    Assert(!IsPalindrom("adam"),__LINE__,"adam");
    Assert(!IsPalindrom("mada"),__LINE__,"mada");
    Assert(!IsPalindrom("faaad"),__LINE__,"faaad");
    // Assert(!IsPalindrom("poap"),__LINE__,"poap");

    Assert(IsPalindrom(""),__LINE__, "empty string is a palindrome");
    Assert(IsPalindrom("a"),__LINE__, "one letter string is a palindrome");
    Assert(IsPalindrom("abba"),__LINE__, "abba is a palindrome");
    Assert(IsPalindrom("abXba"),__LINE__, "abXba is a palindrome");
    Assert(IsPalindrom("a b X b a"),__LINE__, "`a b X b a` is a palindrome");
    Assert(IsPalindrom("  ABBA  "),__LINE__, "`  ABBA  ` is a palindrome");

    Assert(!IsPalindrom("XabbaY"),__LINE__, "XabbaY is not a palindrome");
    Assert(!IsPalindrom("abXYba"),__LINE__, "abXYba is not a palindrome");
    Assert(!IsPalindrom("Xabba"), __LINE__,"Xabba is not a palindrome");
    Assert(!IsPalindrom("abbaX"),__LINE__, "abbaX is not a palindrome");
    Assert(
      !IsPalindrom("was it a car or a cat i saw"),__LINE__,
      "`was it a car or a cat i saw` is not a palindrome because spaces do not match"
    );
    Assert(!IsPalindrom("ABBA   "),__LINE__, "`ABBA   ` is not a palindrome");
    Assert(!IsPalindrom("  ABBA"),__LINE__, "`  ABBA` is not a palindrome");
}

int main() {
  TestRunner runner;
  // добавьте сюда свои тесты
  runner.RunTest(TestPalindrom,"TestPalindrom");
  return 0;
}
