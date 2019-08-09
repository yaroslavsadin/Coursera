#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

#if 0
enum class Gender {
  FEMALE,
  MALE
};

struct Person {
  int age;  // возраст
  Gender gender;  // пол
  bool is_employed;  // имеет ли работу
};

template<typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end);
#endif

void PartitionByAnPrint(vector<Person>& persons, const string msg, function<bool(Person&)> pred) {
  auto it = partition(persons.begin(),persons.end(),pred);
  std::cout << msg << " = " << ComputeMedianAge(persons.begin(), it) << endl;
}

void PrintStats(vector<Person>& persons) {
  PartitionByAnPrint(persons,"Median age",[](const Person& p) {return true;});
  PartitionByAnPrint(persons,"Median age for females",[](const Person& p) {
    return p.gender == Gender::FEMALE;
  });
  PartitionByAnPrint(persons,"Median age for males",[](const Person& p) {
    return p.gender == Gender::MALE;
  });
  PartitionByAnPrint(persons,"Median age for employed females",[](const Person& p) {
    return p.gender == Gender::FEMALE && p.is_employed;
  });
  PartitionByAnPrint(persons,"Median age for unemployed females",[](const Person& p) {
    return p.gender == Gender::FEMALE && !p.is_employed;
  });
  PartitionByAnPrint(persons,"Median age for employed males",[](const Person& p) {
    return p.gender == Gender::MALE && p.is_employed;
  });
  PartitionByAnPrint(persons,"Median age for unemployed males",[](const Person& p) {
    return p.gender == Gender::MALE && !p.is_employed;
  });
}
