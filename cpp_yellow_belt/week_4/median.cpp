#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <exception>
using namespace std;
#if 1
enum class Gender {
  FEMALE,
  MALE
};

struct Person {
  int age;  // возраст
  Gender gender;  // пол
  bool is_employed;  // имеет ли работу
};

ostream& operator<<(ostream& s, const Person& p) {
    auto gend = [](const Person& p) { return (p.gender == Gender::MALE) ? "Male" :"Female"; };
    auto empl = [](const Person& p) { return (p.is_employed) ? "Is employed" : "Is not employed"; };
    return s << p.age << " " << gend(p) << " " << empl(p);
}

// Это пример функции, его не нужно отправлять вместе с функцией PrintStats
template <typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
  if (range_begin == range_end) {
    return 0;
  }
  vector<typename InputIt::value_type> range_copy(range_begin, range_end);
  auto middle = begin(range_copy) + range_copy.size() / 2;
  nth_element(
      begin(range_copy), middle, end(range_copy),
      [](const Person& lhs, const Person& rhs) {
        return lhs.age < rhs.age;
      }
  );
  return middle->age;
}
#endif
template<typename InputIt>
void PrintContainer(InputIt b, InputIt e, char delim = ' ') {
    for(InputIt i = b; i != e; i++) {
        cout << *i << delim;
    }
    cout << endl;
}

void PrintStats(vector<Person> persons) {
    auto males = persons.begin();

    auto lbd_get_fems_point = [](Person& a) {
        if (a.gender == Gender::MALE) {
            return true;
        }
        return false;
    };

    auto lbd_get_unemployed = [](Person& a) {
        if (a.is_employed) {
            return true;
        }
        return false;
    };

    auto fems = partition(persons.begin(),persons.end(),lbd_get_fems_point);
    auto males_unemp = partition(persons.begin(),fems,lbd_get_unemployed);
    auto fems_unemp = partition(fems,persons.end(),lbd_get_unemployed);

    cout << "Median age = " << ComputeMedianAge(persons.begin(),persons.end()) << endl;
    cout << "Median age for females = " << ComputeMedianAge(fems,persons.end()) << endl;
    cout << "Median age for males = " << ComputeMedianAge(males,fems) << endl;
    cout << "Median age for employed females = " << ComputeMedianAge(fems,fems_unemp) << endl;
    cout << "Median age for unemployed females = " << ComputeMedianAge(fems_unemp,persons.end()) << endl;
    cout << "Median age for employed males = " << ComputeMedianAge(persons.begin(),males_unemp) << endl;
    cout << "Median age for unemployed males = " << ComputeMedianAge(males_unemp,fems) << endl;

    // PrintContainer(persons.begin(),persons.end(),'\n');
}

int main() {
  vector<Person> persons = {
      {31, Gender::MALE, false},
      {40, Gender::FEMALE, true},
      {24, Gender::MALE, true},
      {20, Gender::FEMALE, true},
      {80, Gender::FEMALE, false},
      {78, Gender::MALE, false},
      {10, Gender::FEMALE, false},
      {55, Gender::MALE, true},
  };
  PrintStats(persons);
  return 0;
}