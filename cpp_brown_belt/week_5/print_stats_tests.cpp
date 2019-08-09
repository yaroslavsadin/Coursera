#include <iostream>

#include <algorithm>
#include <iostream>
#include "test_runner.h"
#include <string>
#include <vector>

using namespace std;
#if 0
enum class Gender {
  FEMALE,
  MALE
};

struct Person {
  int age;
  Gender gender;
  bool is_employed;
};

bool operator==(const Person& lhs, const Person& rhs) {
  return lhs.age == rhs.age
      && lhs.gender == rhs.gender
      && lhs.is_employed == rhs.is_employed;
}

ostream& operator<<(ostream& stream, const Person& person) {
  return stream << "Person(age=" << person.age 
      << ", gender=" << static_cast<int>(person.gender)
      << ", is_employed=" << boolalpha << person.is_employed << ")";
}

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

vector<Person> ReadPersons(istream& in_stream = cin) {
  int person_count;
  in_stream >> person_count;
  vector<Person> persons;
  persons.reserve(person_count);
  for (int i = 0; i < person_count; ++i) {
    int age, gender, is_employed;
    in_stream >> age >> gender >> is_employed;
    Person person{
        age,
        static_cast<Gender>(gender),
        is_employed == 1
    };
    persons.push_back(person);
  }
  return persons;
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
       ComputeMedianAge(females_end, employed_males_end),
       ComputeMedianAge(employed_males_end, end(persons))
  };
}

void PrintStats(const AgeStats& stats,
                ostream& out_stream = cout) {
  out_stream << "Median age = "
             << stats.total              << endl
             << "Median age for females = "
             << stats.females            << endl
             << "Median age for males = "
             << stats.males              << endl
             << "Median age for employed females = "
             << stats.employed_females   << endl
             << "Median age for unemployed females = "
             << stats.unemployed_females << endl
             << "Median age for employed males = "
             << stats.employed_males     << endl
             << "Median age for unemployed males = "
             << stats.unemployed_males   << endl;
}
#endif

void Test1() {
  {
    const vector<Person> persons;
    int median = ComputeMedianAge(persons.begin(),persons.end());
    cerr << median << endl;
  }
}

void Test2() {
  {
    const vector<Person> persons = {
      {Person{6,Gender::FEMALE,false}},
      {Person{3,Gender::FEMALE,false}},
      {Person{5,Gender::FEMALE,false}},
      {Person{2,Gender::FEMALE,false}},
      {Person{1,Gender::FEMALE,false}},
      {Person{9,Gender::FEMALE,false}},
      {Person{3,Gender::FEMALE,false}},
      {Person{4,Gender::FEMALE,false}},
      {Person{10,Gender::FEMALE,false}}
    };
    int median = ComputeMedianAge(persons.begin(),persons.end());
    // 1 2 3 3 4 5 6 9 10
    const int median_expected {4};
    ASSERT_EQUAL(median,median_expected);
  }
}

void Test3() {
  {
    istringstream is ("1\n66 0 1");
    vector<Person> p = ReadPersons(is);
    ASSERT_EQUAL(p[0].age,66);
    ASSERT_EQUAL(static_cast<int>(p[0].gender),static_cast<int>(Gender::FEMALE));
    ASSERT_EQUAL(p[0].is_employed,1);
  }

}

void Test4() {
  {
    istringstream is ("1\n66 0 1");
    vector<Person> p = ReadPersons(is);
    ASSERT_EQUAL(p[0].age,66);
    ASSERT_EQUAL(static_cast<int>(p[0].gender),static_cast<int>(Gender::FEMALE));
    ASSERT_EQUAL(p[0].is_employed,1);
  }

}

void Test5() {
  vector<Person> persons = {
    {Person{66,Gender::MALE,true}},
    {Person{66,Gender::FEMALE,true}}
  };
  AgeStats stats = ComputeStats(move(persons));
  AgeStats expected = {
  .total = 66,
  .females = 66,
  .males = 66,
  .employed_females = 66,
  .unemployed_females = 0,
  .employed_males = 66,
  .unemployed_males = 0
  };
  ASSERT_EQUAL(expected.employed_males,stats.employed_males);
}

void Test6() {
  vector<Person> persons = {
    {Person{66,Gender::MALE,true}},
    {Person{1,Gender::FEMALE,true}},
    {Person{2,Gender::FEMALE,false}},
    {Person{3,Gender::FEMALE,false}},
    {Person{4,Gender::FEMALE,false}},
    {Person{5,Gender::FEMALE,false}},
    {Person{66,Gender::MALE,false}}
  };
  AgeStats stats = ComputeStats(move(persons));
  AgeStats expected = {
  .total = 66,
  .females = 3,
  .males = 66,
  .employed_females = 1,
  .unemployed_females = 3,
  .employed_males = 66,
  .unemployed_males = 66
  };
  ASSERT_EQUAL(expected.employed_males,stats.employed_males);
}

int main(void) {
    TestRunner tr;
    RUN_TEST(tr,Test1);
    RUN_TEST(tr,Test2);
    RUN_TEST(tr,Test3);
    RUN_TEST(tr,Test4);
    RUN_TEST(tr,Test5);
    RUN_TEST(tr,Test6);
}