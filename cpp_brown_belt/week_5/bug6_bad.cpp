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

    // cout << "Median age = "
    //    << ComputeMedianAge(begin(persons), end(persons))         << endl
    //    << "Median age for females = "
    //    << ComputeMedianAge(begin(persons), females_end)          << endl
    //    << "Median age for males = "
    //    << ComputeMedianAge(females_end, end(persons))            << endl
    //    << "Median age for employed females = "
    //    << ComputeMedianAge(begin(persons), employed_females_end) << endl
    //    << "Median age for unemployed females = "
    //    << ComputeMedianAge(employed_females_end, females_end)    << endl
    //    << "Median age for employed males = "
    //    << ComputeMedianAge(females_end, employed_males_end)      << endl
    //    << "Median age for unemployed males = "
    //    << ComputeMedianAge(employed_males_end, end(persons))     << endl;
  
  return {
       ComputeMedianAge(begin(persons), end(persons)),
       ComputeMedianAge(females_end, end(persons)),
       ComputeMedianAge(begin(persons), females_end),
       ComputeMedianAge(females_end, employed_males_end),
       ComputeMedianAge(employed_males_end, end(persons)),
       ComputeMedianAge(begin(persons), employed_females_end),
       ComputeMedianAge(employed_females_end, females_end)
  };
}

void Test6() {
  vector<Person> persons{
    {Person{2,Gender::FEMALE,true}},
    {Person{18,Gender::FEMALE,true}},
    {Person{91,Gender::FEMALE,true}},
    
    {Person{1,Gender::FEMALE,false}},
    {Person{2,Gender::FEMALE,false}},
    {Person{5,Gender::FEMALE,false}},
    {Person{9,Gender::FEMALE,false}},
    {Person{13,Gender::FEMALE,false}},
    {Person{22,Gender::FEMALE,false}},
    {Person{23,Gender::FEMALE,false}},

    {Person{4,Gender::MALE,true}},
    {Person{6,Gender::MALE,true}},
    {Person{7,Gender::MALE,true}},
    {Person{14,Gender::MALE,true}},
    {Person{15,Gender::MALE,true}},
    
    
    {Person{10,Gender::MALE,false}},
    {Person{100,Gender::MALE,false}},
    {Person{150,Gender::MALE,false}}
  };

  AgeStats stats = ComputeStats(persons);
  ASSERT_EQUAL(stats.total,13);
  ASSERT_EQUAL(stats.females,13);
  ASSERT_EQUAL(stats.males,14);
  ASSERT_EQUAL(stats.unemployed_females,9);
  ASSERT_EQUAL(stats.employed_males,7);
  ASSERT_EQUAL(stats.unemployed_males,100);

  ostringstream os;

  // PrintStats(stats,os);

  ASSERT_EQUAL(os.str(),
  "Median age = 13\n" \
  "Median age for females = 13\n" \
  "Median age for males = 14\n" \
  "Median age for employed females = 18\n" \
  "Median age for unemployed females = 9\n" \
  "Median age for employed males = 7\n" \
  "Median age for unemployed males = 100\n"
  );
}

int main() {
  TestRunner tr;
  RUN_TEST(tr,Test6);
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
       << "Median age for males = "
       << ComputeMedianAge(females_end, end(persons))            << endl
       << "Median age for females = "
       << ComputeMedianAge(begin(persons), females_end)          << endl
       << "Median age for employed males = "
       << ComputeMedianAge(females_end, employed_males_end)      << endl
       << "Median age for unemployed males = "
       << ComputeMedianAge(employed_males_end, end(persons))     << endl
       << "Median age for employed females = "
       << ComputeMedianAge(begin(persons), employed_females_end) << endl
       << "Median age for unemployed females = "
       << ComputeMedianAge(employed_females_end, females_end)    << endl;

  return 0;
}

