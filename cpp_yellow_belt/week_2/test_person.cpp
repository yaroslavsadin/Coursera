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

template <class K, class V>
ostream& operator << (ostream& os, const pair<K, V>& p) {
  os << "(";
  os << p.first << ", " << p.second;
  return os << ")";
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

#if 1
class Person {
public:
  // Вы можете вставлять сюда различные реализации класса,
  // чтобы проверить, что ваши тесты пропускают корректный код
  // и ловят некорректный
  void ChangeFirstName(int year, const string& first_name) {
    data[year].first = first_name;
  }
  void ChangeLastName(int year, const string& last_name) {
    data[year].second = last_name;
  }
  string GetFullName(int year) {
    string first_name, last_name;
    for(const auto& [year_,name] : data) {
      if(year_ > year) break;
      if(!name.first.empty())  
        first_name = name.first;
      if(!name.second.empty())
        last_name = name.second;
    }
    if(first_name.empty() && last_name.empty()) {
      return "Incognito";
    } else if(first_name.empty()) {
      return last_name+" with unknown first name";
    } else if(last_name.empty()) {
      return first_name+" with unknown last name";
    } else {
      return first_name+" "+last_name;
    }
  }
private:
  map<int,pair<string,string>> data;
};
#endif

  void TestChangeFirstName() {
  {
    Person p;
    p.ChangeFirstName(1990,"Yaroslav");
    string expected = "Yaroslav with unknown last name";
    string result = p.GetFullName(1990);
    AssertEqual(expected,result,__LINE__,"Added Yaroslav to 1990");
  }
  {
    Person p;
    p.ChangeFirstName(1991,"Vasya");
    string expected = "Vasya with unknown last name";
    string result = p.GetFullName(1991);
    AssertEqual(expected,result,__LINE__,"Added Vasya to 1991");
  }
}

void TestChangeLastName() {
  {
    Person p;
    p.ChangeLastName(1990,"Sadin");
    string expected = "Sadin with unknown first name";
    string result = p.GetFullName(1990);
    AssertEqual(expected,result,__LINE__,"Added Sadin to 1990");
  }
  {
    Person p;
    p.ChangeLastName(1991,"Ugryumov");
    string expected = "Ugryumov with unknown first name";
    string result = p.GetFullName(1991);
    AssertEqual(expected,result,__LINE__,"Added Ugryumov to 1991");
  }
}

void TestGetFullName() {
  {
    Person p;
    p.ChangeLastName(1950,"Ugryumov");
    p.ChangeLastName(1970,"Marin");
    p.ChangeFirstName(1980,"Alexander");
    p.ChangeLastName(1990,"Sadin");
    p.ChangeFirstName(1999,"Yaroslav");
    p.ChangeLastName(1999,"Zadin");
    p.ChangeLastName(2007,"Sadin");

    string expected, result;

    expected = "Incognito";
    result = p.GetFullName(1900);
    AssertEqual(expected,result,__LINE__,"Incognito not working");

    expected = "Alexander Marin";
    result = p.GetFullName(1980);
    AssertEqual(expected,result,__LINE__,"");

    expected = "Yaroslav Zadin";
    result = p.GetFullName(1999);
    AssertEqual(expected,result,__LINE__,"");

    expected = "Yaroslav Sadin";
    result = p.GetFullName(2007);
    AssertEqual(expected,result,__LINE__,"");

    expected = "Yaroslav Sadin";
    result = p.GetFullName(2100);
    AssertEqual(expected,result,__LINE__,"Year more than Person contains");
  }
}

int main() {
  TestRunner runner;
  
  runner.RunTest(TestChangeFirstName,"PersonTestChangeFirstName");
  runner.RunTest(TestChangeLastName,"PersonTestChangeLastName");
  runner.RunTest(TestGetFullName,"PersonTestGetFullName");
  // добавьте сюда свои тесты
  return 0;
}
