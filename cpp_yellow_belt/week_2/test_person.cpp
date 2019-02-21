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

  void TestChangeFirstName() {
    using TestType = map<int,pair<string,string>>;
    {
      TestType expected = {
        {1990, {"Yaroslav",""}}
      };
      ChangeFirstName(1990,"Yaroslav");
      AssertEqual(expected,data,__LINE__,"Added self");
    }
    {
      TestType expected = {
        {1990, {"Vasya",""}}
      };
      ChangeFirstName(1990,"Vasya");
      AssertEqual(expected,data,__LINE__,"");
    }
    {
      TestType expected = {
        {1990, {"Vasya",""}},
        {1980, {"Petya",""}}
      };
      ChangeFirstName(1980,"Petya");
      AssertEqual(expected,data,__LINE__,"");
    }
  }

  void TestChangeLastName() {
    using TestType = map<int,pair<string,string>>;
    {
      TestType expected = {
        {1990, {"","Sadin"}}
      };
      ChangeLastName(1990,"Sadin");
      AssertEqual(expected,data,__LINE__,"Added self");
    }
    {
      TestType expected = {
        {1990, {"","Ugryumov"}}
      };
      ChangeLastName(1990,"Ugryumov");
      AssertEqual(expected,data,__LINE__,"");
    }
    {
      TestType expected = {
        {1990, {"","Ugryumov"}},
        {1980, {"","Marin"}}
      };
      ChangeLastName(1980,"Marin");
      AssertEqual(expected,data,__LINE__,"");
    }
  }

  void TestGetFullName() {
    data = {
        {1950, {"","Ugryumov"}},
        {1970, {"","Marin"}},
        {1980, {"Alexander",""}},
        {1990, {"","Sadin"}},
        {1999, {"Yaroslav","Zadin"}},
        {2007, {"","Sadin"}}        
      };
    { 
      string expected = "Incognito";
      string result = GetFullName(1900);
      AssertEqual(expected,result,__LINE__,"Incognito");
    }
    { 
      string expected = "Ugryumov with unknown first name";
      string result = GetFullName(1950);
      AssertEqual(expected,result,__LINE__,"unknown first name");
    }
    {
      string expected = "Alexander Sadin";
      string result = GetFullName(1990);
      AssertEqual(expected,result,__LINE__,"Both");
    }
    {
      string expected = "Yaroslav Sadin";
      string result = GetFullName(2007);
      AssertEqual(expected,result,__LINE__,"Both");
    }
    { 
      string expected = "Alexander with unknown last name";
      data.erase(1950);
      data.erase(1970);
      string result = GetFullName(1980);
      AssertEqual(expected,result,__LINE__,"unknown last name");
    }
  }
private:
  map<int,pair<string,string>> data;
};

void PersonTestChangeFirstName() {
  Person p;
  p.TestChangeFirstName();
}
void PersonTestChangeLastName() {
  Person p;
  p.TestChangeLastName();
}
void PersonTestGetFullName() {
  Person p;
  p.TestGetFullName();
}

int main() {
  TestRunner runner;
  
  runner.RunTest(PersonTestChangeFirstName,"PersonTestChangeFirstName");
  runner.RunTest(PersonTestChangeLastName,"PersonTestChangeLastName");
  runner.RunTest(PersonTestGetFullName,"PersonTestGetFullName");
  // добавьте сюда свои тесты
  return 0;
}
