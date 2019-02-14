#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

class Person {
public:
  Person(string first_name,string last_name,int birth_year) : birth_year(birth_year) {
    year_to_name[birth_year] = {first_name,last_name};
  }
  void ChangeFirstName(int year, const string& first_name) {
    // добавить факт изменения имени на first_name в год year
    if (year < birth_year) {
      return;
    }
    year_to_name[year].first_name = first_name;
  }
  void ChangeLastName(int year, const string& last_name) {
    // добавить факт изменения фамилии на last_name в год year
    if (year < birth_year) {
      return;
    }
    year_to_name[year].last_name = last_name;
  }
  string GetFullName(int year) const {
    if (year < birth_year) {
      return "No person";
    }
    // получить имя и фамилию по состоянию на конец года year
    string first_name, last_name;
    for(const auto& i : year_to_name) {
        if(i.first <= year) {
            if(i.second.first_name.size()) first_name = i.second.first_name;
            if(i.second.last_name.size()) last_name = i.second.last_name;
        }
    }
    return first_name+" "+last_name;
  }
  // TODO: To avoid duplicating the code it would be better to write functions FindNamesHistory, BuildJoinedName, BuildFullName
  string GetFullNameWithHistory(int year) const {
    if (year < birth_year) {
      return "No person";
    }
    // получить имя и фамилию по состоянию на конец года year
    vector<string> first_names, last_names;
    for(const auto& i : year_to_name) {
        if(i.first <= year) {
            if(i.second.first_name.size()) {
                if(!first_names.empty() && first_names.back() == i.second.first_name)
                    continue;
                first_names.push_back(i.second.first_name);
            }
            if(i.second.last_name.size()) {
                if(!last_names.empty() && last_names.back() == i.second.last_name)
                    continue;
                last_names.push_back(i.second.last_name);
            }
        }
    }
    return GetNames(first_names)+" "+GetNames(last_names);
  }
private:
  // приватные поля

    string GetNames(vector<string>& names) const {
        if(names.empty()) 
            return "";
        if(names.size() == 1) 
            return names[0];
        string last_name = names.back(); names.pop_back();
        last_name += " (";
        for(int i = names.size()-1; i >= 1; i--) {
            last_name += names[i]+", ";
        }
        last_name += names[0]+")";
        return last_name;
    }

    struct FullName {
        string first_name;
        string last_name;
    };

  map<int,FullName> year_to_name;
  int birth_year;
};

int main(void) {
    Person person("Polina", "Sergeeva", 1960);
    for (int year : {1959, 1960}) {
      cout << person.GetFullNameWithHistory(year) << endl;
    }
    
    person.ChangeFirstName(1965, "Appolinaria");
    person.ChangeLastName(1967, "Ivanova");
    for (int year : {1965, 1967}) {
      cout << person.GetFullNameWithHistory(year) << endl;
    }

    return 0;
}