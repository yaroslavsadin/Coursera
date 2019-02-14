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
private:
  // приватные поля

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