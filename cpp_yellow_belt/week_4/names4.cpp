#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <exception>
#include "test_runner.h"
using namespace std;

class Person {
public:
  void ChangeFirstName(int year, const string& first_name) {
    // добавить факт изменения имени на first_name в год year
    names[year].first = first_name;
  }
  void ChangeLastName(int year, const string& last_name) {
    // добавить факт изменения фамилии на last_name в год year
    names[year].second = last_name;
  }
  string GetFullName(int year) {
    // получить имя и фамилию по состоянию на конец года year
    // с помощью двоичного поиска
    auto p = names.equal_range(year);
    auto found = p.first;
    auto after_found = p.second;

    if(found == after_found && after_found == names.begin()) {
      return "Incognito";
    } else {
      string first_name;
      string last_name;
      
      auto rfound = make_reverse_iterator(after_found);
      auto f = find_if(rfound,names.rend(),[](auto& p) {
        return (p.second.first.size() != 0);
      });

      if(f != names.rend())  
        first_name = f->second.first;

      auto s = find_if(rfound,names.rend(),[](auto& p) {
        return (p.second.second.size() != 0);
      });

      if(s != names.rend()) 
        last_name = s->second.second;

      if (!first_name.size()) {
          return last_name+" with unknown first name";
      } else if (!last_name.size()) {
          return first_name+" with unknown last name";
      } else {
          return first_name+" "+last_name;
      }
    }    
  }
private:
  // приватные поля
  // silly \/\/\/
  map<int,pair<string,string>> names;
  // map<int, string> first_names;
  // map<int, string> last_names;
};

 void TestChangeFirstName() {
  {
    Person p;
    p.ChangeFirstName(1990,"Yaroslav");
    string expected = "Yaroslav with unknown last name";
    string result = p.GetFullName(1990);
    AssertEqual(expected,result,"Added Yaroslav to 1990",__LINE__,__FILE__);
  }
  {
    Person p;
    p.ChangeFirstName(1991,"Vasya");
    string expected = "Vasya with unknown last name";
    string result = p.GetFullName(1991);
    AssertEqual(expected,result,"Added Vasya to 1991",__LINE__,__FILE__);
  }
}

void TestChangeLastName() {
  {
    Person p;
    p.ChangeLastName(1990,"Sadin");
    string expected = "Sadin with unknown first name";
    string result = p.GetFullName(1990);
    AssertEqual(expected,result,"Added Sadin to 1990",__LINE__,__FILE__);
  }
  {
    Person p;
    p.ChangeLastName(1991,"Ugryumov");
    string expected = "Ugryumov with unknown first name";
    string result = p.GetFullName(1991);
    AssertEqual(expected,result,"Added Ugryumov to 1991",__LINE__,__FILE__);
  }
}

void TestGetFullName() {
  {
    Person p;
    string expected, result;
    expected = "Ugryumov with unknown first name";
    p.ChangeLastName(1950,"Ugryumov");
    result = p.GetFullName(1960);
    AssertEqual(expected,result,"Ugryumov with unknown first name",__LINE__,__FILE__);
  }
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
    AssertEqual(expected,result,"Incognito not working",__LINE__,__FILE__);

    expected = "Alexander Marin";
    result = p.GetFullName(1980);
    AssertEqual(expected,result,"",__LINE__,__FILE__);

    expected = "Yaroslav Zadin";
    result = p.GetFullName(1999);
    AssertEqual(expected,result,"",__LINE__,__FILE__);

    expected = "Yaroslav Sadin";
    result = p.GetFullName(2007);
    AssertEqual(expected,result,"",__LINE__,__FILE__);

    expected = "Yaroslav Sadin";
    result = p.GetFullName(2100);
    AssertEqual(expected,result,"Year more than Person contains",__LINE__,__FILE__);
  }
}

int main() {
  {
    TestRunner runner;
  
    runner.RunTest(TestChangeFirstName,"PersonTestChangeFirstName");
    runner.RunTest(TestChangeLastName,"PersonTestChangeLastName");
    runner.RunTest(TestGetFullName,"PersonTestGetFullName");
  }

  Person person;
  
  person.ChangeFirstName(1965, "Polina");
  person.ChangeLastName(1967, "Sergeeva");
  for (int year : {1900, 1965, 1990}) {
    cout << person.GetFullName(year) << endl;
  }
  
  person.ChangeFirstName(1970, "Appolinaria");
  for (int year : {1969, 1970}) {
    cout << person.GetFullName(year) << endl;
  }
  
  person.ChangeLastName(1968, "Volkova");
  for (int year : {1969, 1970}) {
    cout << person.GetFullName(year) << endl;
  }
  
  return 0;
}
