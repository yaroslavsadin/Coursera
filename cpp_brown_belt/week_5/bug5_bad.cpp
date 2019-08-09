#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include"test_runner.h"

using namespace std;

enum class Gender {
  FEMALE,
  MALE
};

struct Person {
  int age;
  Gender gender;
  bool is_employed;
};

struct AgeStats {
  int total;
  int females;
  int males;
  int employed_females;
  int unemployed_females;
  int employed_males;
  int unemployed_males;
};

template <typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
  if (range_begin == range_end) {
    return 0;
  }
  vector<typename iterator_traits<InputIt>::value_type> range_copy(
      range_begin,
      range_end
  );
  auto middle = begin(range_copy) + range_copy.size() / 2;
  nth_element(
      begin(range_copy), middle, end(range_copy),
      [](const Person& lhs, const Person& rhs) {
        return lhs.age < rhs.age;
      }
  );
  return middle->age;
}

AgeStats ComputeStats(vector<Person> persons) {
  //                 persons
  //                |       |
  //          females        males
  //         |       |      |     |
  //      empl.  unempl. empl.   unempl.

  auto females_end = partition(
      begin(persons), end(persons),
      [](const Person& p) {
        return p.gender == Gender::FEMALE;
      }
  );
  auto employed_females_end = partition(
      begin(persons), females_end,
      [](const Person& p) {
        return p.is_employed;
      }
  );
  auto employed_males_end = partition(
      females_end, end(persons),
      [](const Person& p) {
        return p.is_employed;
      }
  );

  return {
       ComputeMedianAge(begin(persons), end(persons)),
       ComputeMedianAge(begin(persons), females_end),
       ComputeMedianAge(females_end, end(persons)),
       ComputeMedianAge(begin(persons),  employed_females_end),
       ComputeMedianAge(employed_females_end, females_end),
       ComputeMedianAge(employed_males_end, females_end),
       ComputeMedianAge(employed_males_end, end(persons))
  };
}

void Test5() {
  vector<Person> persons{
    {Person{42,Gender::FEMALE,false}},
    {Person{43,Gender::FEMALE,false}},
    {Person{44,Gender::FEMALE,false}},
    {Person{45,Gender::FEMALE,false}},
    {Person{46,Gender::MALE,true}},
    {Person{47,Gender::MALE,false}},
    {Person{48,Gender::MALE,true}},
    {Person{49,Gender::MALE,true}},
    {Person{50,Gender::MALE,false}}
  };
  AgeStats stats;
  try{  
    stats = ComputeStats(persons);
  } catch (exception& e) {
    cerr << e.what() << endl;
    ASSERT(0);
  }
  ASSERT_EQUAL(stats.employed_males,48);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr,Test5);
  return 0;
  int person_count;
  cin >> person_count;
  vector<Person> persons;
  persons.reserve(person_count);
  for (int i = 0; i < person_count; ++i) {
    int age, gender, is_employed;
    cin >> age >> gender >> is_employed;
    Person person{
        age,
        static_cast<Gender>(gender),
        is_employed == 1
    };
    persons.push_back(person);
  }

  auto females_end = partition(
      begin(persons), end(persons),
      [](const Person& p) {
        return p.gender == Gender::FEMALE;
      }
  );
  auto employed_females_end = partition(
      begin(persons), females_end,
      [](const Person& p) {
        return p.is_employed;
      }
  );
  auto employed_males_end = partition(
      females_end, end(persons),
      [](const Person& p) {
        return p.is_employed;
      }
  );

  cout << "Median age = "
       << ComputeMedianAge(begin(persons), end(persons))         << endl
       << "Median age for females = "
       << ComputeMedianAge(begin(persons), females_end)          << endl
       << "Median age for males = "
       << ComputeMedianAge(females_end, end(persons))            << endl
       << "Median age for employed females = "
       << ComputeMedianAge(begin(persons), employed_females_end) << endl
       << "Median age for unemployed females = "
       << ComputeMedianAge(employed_females_end, females_end)    << endl
       << "Median age for employed males = "
       << ComputeMedianAge(employed_males_end, females_end)      << endl
       << "Median age for unemployed males = "
       << ComputeMedianAge(employed_males_end, end(persons))     << endl;

  return 0;
}

