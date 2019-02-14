#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

class Person {
public:
  void ChangeFirstName(int year, const string& first_name) {
    // добавить факт изменения имени на first_name в год year
    year_to_name[year].first_name = first_name;
  }
  void ChangeLastName(int year, const string& last_name) {
    // добавить факт изменения фамилии на last_name в год year
    year_to_name[year].last_name = last_name;
  }
  string GetFullName(int year) {
    // получить имя и фамилию по состоянию на конец года year
    string first_name, last_name;
    for(const auto& i : year_to_name) {
        if(i.first <= year) {
            if(i.second.first_name.size()) first_name = i.second.first_name;
            if(i.second.last_name.size()) last_name = i.second.last_name;
        }
    }
    if (!first_name.size()) {
        if(!last_name.size()) {
            return "Incognito";
        } else {
            return last_name+" with unknown first name";
        }
    } else if (!last_name.size()) {
        return first_name+" with unknown last name";
    } else {
        return first_name+" "+last_name;
    }
  }
  // TODO: To avoid duplicating the code it would be better to write functions FindNamesHistory, BuildJoinedName, BuildFullName
  string GetFullNameWithHistory(int year) {
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
    if (first_names.empty()) {
        if(last_names.empty()) {
            return "Incognito";
        } else {
            return GetNames(last_names)+" with unknown first name";
        }
    } else if (last_names.empty()) {
        return GetNames(first_names)+" with unknown last name";
    } else {
        return GetNames(first_names)+" "+GetNames(last_names);
    }
  }
private:
  // приватные поля

    string GetNames(vector<string>& names) {
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
};

int main(void) {
    Person person;
  
  person.ChangeFirstName(1965, "Polina");
  person.ChangeLastName(1967, "Sergeeva");
  for (int year : {1900, 1965, 1990}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }
  
  person.ChangeFirstName(1970, "Appolinaria");
  for (int year : {1969, 1970}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }
  
  person.ChangeLastName(1968, "Volkova");
  for (int year : {1969, 1970}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }
  
  person.ChangeFirstName(1990, "Polina");
  person.ChangeLastName(1990, "Volkova-Sergeeva");
  cout << person.GetFullNameWithHistory(1990) << endl;
  
  person.ChangeFirstName(1966, "Pauline");
  cout << person.GetFullNameWithHistory(1966) << endl;
  
  person.ChangeLastName(1960, "Sergeeva");
  for (int year : {1960, 1967}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }
  
  person.ChangeLastName(1961, "Ivanova");
  cout << person.GetFullNameWithHistory(1967) << endl;
    return 0;
}