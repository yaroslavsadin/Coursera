#include <iostream>

#include <algorithm>
#include <iostream>
#include "test_runner.h"
#include <string>
#include <vector>

using namespace std;
#if 1
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

void Test1(void) {
  vector<Person> persons;
  ASSERT_EQUAL(ComputeMedianAge(begin(persons), end(persons)), 0);
}

void Test2(void) {
  vector<Person> test_middle = {
    {Person{2,Gender::FEMALE,false}},
    {Person{7,Gender::FEMALE,false}},
    {Person{4,Gender::FEMALE,false}},
    {Person{9,Gender::FEMALE,false}},
    {Person{8,Gender::FEMALE,false}},
    {Person{3,Gender::FEMALE,false}},
    {Person{1,Gender::FEMALE,false}},
  };
  int age = ComputeMedianAge(test_middle.begin(),test_middle.end());
  // Expecting segfault
  ASSERT_EQUAL(age,4);
}

void Test3() {
  istringstream is("2\n \
  42 0 1\n              \
  43 1 0");
  vector<Person> persons = ReadPersons(is);
  ASSERT_EQUAL(persons.size(),2u);
  ASSERT_EQUAL(static_cast<int>(persons[0].gender),static_cast<int>(Gender::FEMALE));
  ASSERT_EQUAL(static_cast<int>(persons[1].gender),static_cast<int>(Gender::MALE));
}

void Test4() {
  istringstream is("2\n \
  42 0 1\n              \
  43 1 0");
  vector<Person> persons = ReadPersons(is);
  ASSERT_EQUAL(persons.size(),2u);
  ASSERT_EQUAL(persons[0].is_employed,true);
  ASSERT_EQUAL(persons[1].is_employed,false);
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

  PrintStats(stats,os);

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

int main(void) {
    TestRunner tr;
    RUN_TEST(tr,Test1);
    RUN_TEST(tr,Test2);
    RUN_TEST(tr,Test3);
    RUN_TEST(tr,Test4);
    RUN_TEST(tr,Test5);
    RUN_TEST(tr,Test6);
}