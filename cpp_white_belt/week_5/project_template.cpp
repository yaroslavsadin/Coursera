#include <iostream>
#include <map>
#include <string>
#include <set>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

// Реализуйте функции и методы классов и при необходимости добавьте свои

class Date {
public:
  Date() {}
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
    // cout << "Destruction here: " << year << " " << month << " " << day << endl; 
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
    // cout << "Construction here: " << year << "-" << month << "-" << day << endl;
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
  stream << setw(4) << setfill('0') << date.GetYear() << "-" << setw(2) << date.GetMonth() << "-" << setw(2) << date.GetDay();
  return stream;
}

istream& operator>>(istream& stream, Date& date) {
  string s;
  stream >> s;
  date = Date(s);
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
    int res = vault.at(date).size();
    vault.erase(date);
    return res;
  }

const set<string>& Find(const Date& date) const {
  if (vault.count(date)) {
    return vault.at(date);
  } else {
    /*
    throw(logic_error("Date doesn't exist"));
    */
    return dummy;
  }
}
  
  void Print() const {
    for(const auto& date : vault) {
      for(const auto& event : date.second) {
        cout << date.first << " " << event << endl;
      }
    }
  }
private:
  map<Date,set<string>> vault;
  set<string> dummy;
};

string ParseCommand(const string& command, Database& db) {
  stringstream ss{command};
  string opcode;
  ss >> opcode;
  if(opcode == "Add") {
    Date date;
    string event;
    ss >> date >> event;
    db.AddEvent(date,event);
    return "";
  } else if (opcode == "Del") {
    Date date;
    string event;

    ss >> date;
    if (ss >> event) {
      if (db.DeleteEvent(date,event)) {
        return "Deleted succesfully";
      } else {
        return "Event not found";
      }
    } else {
      return "Deleted "+to_string(db.DeleteDate(date))+" events";
    }
  } else if(opcode == "Print") {
    db.Print();
    return "";
  } else if(opcode == "Find") {
    Date date;
    ss >> date;
    string res;
    set<string> events = db.Find(date);
    for(const auto& event : events) {
      res += event+"\n";
    }
    // remove last \n
    res = res.substr(0,res.size()-1);
    return res;
  } else if(opcode == ""){
    return "";
  } else {
    throw(runtime_error("Unknown command: "+command));
  }
}

int main() {
  Database db;
#if 0
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
      auto date = Date("2016--4-32");
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
    try {
      auto date = Date("2016-13-uuu32");
      cout << date << endl;
    } catch(runtime_error& e) {
      cout << e.what() << endl;
    }
    try {
      auto date = Date("23eef344gt65");
      cout << date << endl;
    } catch(runtime_error& e) {
      cout << e.what() << endl;
    }
    try {
      stringstream ss("1--1-0");
      auto date = Date("1970-1-1");
      ss >> date;
    } catch(runtime_error& e) {
      cout << e.what() << endl;
    }
    try {
      stringstream ss("1965-4-31");
      auto date = Date("1970-1-1");
      ss >> date;
      cout << date << endl;
    } catch(runtime_error& e) {
      cout << e.what() << endl;
    }
  }
  {
    try {  
      db.AddEvent({"1990-06-21"},"Birthday");
      db.AddEvent({"1997-09-1"},"School");
      db.AddEvent({"1997-09-1"},"End-of-summer");
      db.AddEvent({"2001-01-1"},"Millenium");
      db.AddEvent({"2001-01-1"},"New-Year");
      db.AddEvent({"2001-01-1"},"New-Year");
      db.AddEvent({"2007-07-1"},"SchoolEnd");
      db.Print();
      db.AddEvent({"2007-07-1"},"UniversityExams");
      db.DeleteEvent({"2007-07-1"},"UniversityExams");
      db.DeleteEvent({"2007-07-1"},"First-Girlfriend");
      db.DeleteDate({"2001-01-1"});
      auto events = db.Find({"2005-12-21"});
      for(const string& event : events) {
        cout << event << endl;
      }
      for(const string& event : db.Find({"1997-09-01"})) {
        cout << event << endl;
      }
    } catch(exception& e) {
      cout << e.what() << endl;
    }
  }
#endif
  string command;
  while (getline(cin, command)) {
    // Считайте команды с потока ввода и обработайте каждую
    string feedback;
    try {  
       feedback = ParseCommand(command, db);
    } catch(exception& e) {
      cout << e.what() << endl;
      return -1;
    }
    if(feedback != "") { 
      cout << feedback << endl;
    }
  }

  return 0;
}