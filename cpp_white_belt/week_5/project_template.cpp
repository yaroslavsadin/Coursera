#include <iostream>
#include <map>
#include <string>
#include <set>
#include <fstream>
#include <sstream>
using namespace std;

// Реализуйте функции и методы классов и при необходимости добавьте свои

class Date {
public:
  Date(int year, int month, int day) : year(year), month(month), day(day) {
    DateValueChecker(year, month, day);
  }
  Date(string date) {
    stringstream ss{date};
    int y,m,d;
    if (!(ss >> y)) {
      throw(runtime_error("Wrong date format: "+date));
    }
    if(ss.peek() != '-') {
      throw(runtime_error("Wrong date format: "+date));
    }
    ss.ignore(1);
    if(!(ss >> m)) {
      throw(runtime_error("Wrong date format: "+date));
    }
    if(ss.peek() != '-') {
      throw(runtime_error("Wrong date format: "+date));
    }
    ss.ignore(1);
    if(!(ss >> d)) {
      throw(runtime_error("Wrong date format: "+date));
    }
    year = y; month = m; day = d;
    DateValueChecker(year, month, day);
  }
  ~Date() {
    cout << "Destruction here: " << year << " " << month << " " << day << endl; 
  }
  int GetYear() const {
    return year;
  }
  int GetMonth() const {
    return month;
  }
  int GetDay() const {
    return day;
  }
private:
  void DateValueChecker(int year, int month, int day) {
    cout << "Construction here: " << year << "-" << month << "-" << day << endl;
    if(month < 1 || month > 12) {
      throw(runtime_error("Month value is invalid: "+to_string(month)));
    } else if (day < 1 || day > 31) {
      throw(runtime_error("Day value is invalid: "+to_string(day)));
    }
  }
  int year,month,day;
};

bool operator<(const Date& lhs, const Date& rhs) {
  if(lhs.GetYear() < rhs.GetYear()) {
    return true;
  } else if (lhs.GetYear() == rhs.GetYear()) {
    if(lhs.GetMonth() < rhs.GetMonth()) {
      return true;
    } else if (lhs.GetMonth() == rhs.GetMonth()) {
      return lhs.GetDay() < rhs.GetDay();
    } else {
      return false;
    }
  } else {
    return false;
  }
}

ostream& operator<<(ostream& stream, const Date& date) {
  stream << date.GetYear() << "-" << date.GetMonth() << "-" << date.GetDay();
  return stream;
}

class Database {
public:
  void AddEvent(const Date& date, const string& event) {
    vault[date].insert(event);
  }
  bool DeleteEvent(const Date& date, const string& event) {
    return vault[date].erase(event);
  }
  int  DeleteDate(const Date& date) {
    return vault.erase(date);
  }

const set<string>& Find(const Date& date) const {
  if (vault.count(date)) {
    return vault.at(date);
  }
}
  
  void Print() const;
private:
  map<Date,set<string>> vault;
};

int main() {
  Database db;

  { 
    try {
      auto date = Date("2016-10-05");
      cout << date << endl;
    } catch(runtime_error& e) {
      cout << e.what() << endl;
    }
    try {
      auto date =  Date("2016-05-10");
      cout << date << endl;
    } catch(runtime_error& e) {
      cout << e.what() << endl;
    }
    try {
      auto date =  Date("0-10-05");
      cout << date << endl;
    } catch(runtime_error& e) {
      cout << e.what() << endl;
    }
    try {
      auto date = Date("2016-13-05");
      cout << date << endl;
    } catch(runtime_error& e) {
      cout << e.what() << endl;
    }
    try {
      auto date = Date("2016-12-32");
      cout << date << endl;
    } catch(runtime_error& e) {
      cout << e.what() << endl;
    }
    try {
      auto date = Date("2016----13-32");
      cout << date << endl;
    } catch(runtime_error& e) {
      cout << e.what() << endl;
    }
  }
  // string command;
  // while (getline(cin, command)) {
  //   // Считайте команды с потока ввода и обработайте каждую
  // }

  return 0;
}