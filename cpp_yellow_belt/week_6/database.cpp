#include "database.h"
#include <iterator>
#include <algorithm>
#include <iostream>
#include "test_framework.h"

void Database::Add(Date date, string event) {
    db[date].insert(event);
}

void Database::Print(const ostream& os) const {
    for(const auto& date : db) {
        for(const auto& event : date.second) {
        cout << date.first << " " << event << endl;
        }
    }
}

string Database::Last(Date date) {
    if (db.empty() || date < db.begin()->first) {
        return "No entries";
    }
    auto it = upper_bound(db.begin(),db.end(),date,[](const Date &a, const pair<Date,set<string>> &b) {
         return a < b.first; 
    });
    if (it != db.begin()) {
        it = prev(it);
    }
    return *prev(it->second.end());
}

#ifdef TEST_DATABASE
void TestDatabase(void) {
    Database db;
    {
        string expected = "No entries";
        string result = db.Last({1995,5,1});
        AssertEqual(result,expected,"Last return wrong value",__LINE__,__FILE__);
    }
    db.Add(Date{1990,6,21},"Birthday");
    db.Add(Date{1997,9,1},"School");
    db.Add(Date{1997,9,1},"End-of-summer");
    db.Add(Date{2001,1,1},"Millenium");
    db.Add(Date{2001,1,1},"New-Year");
    db.Add(Date{2001,1,1},"New-Year");
    db.Add(Date{2007,7,1},"SchoolEnd");
    db.Add(Date{2012,7,15},"Employed");
    {    
        string expected = "No entries";
        string result = db.Last({1990,6,20});
        AssertEqual(result,expected,"Last return wrong value",__LINE__,__FILE__);
    }
    {    
        string expected = "Birthday";
        string result = db.Last({1990,6,21});
        AssertEqual(result,expected,"Last return wrong value",__LINE__,__FILE__);
    }
    {    
        string expected = "Birthday";
        string result = db.Last({1995,5,1});
        AssertEqual(result,expected,"Last return wrong value",__LINE__,__FILE__);
    }
    {    
        string expected = "SchoolEnd";
        string result = db.Last({2008,1,1});
        AssertEqual(result,expected,"Last return wrong value",__LINE__,__FILE__);
    }
    {    
        string expected = "Employed";
        string result = db.Last({2012,7,16});
        AssertEqual(result,expected,"Last return wrong value",__LINE__,__FILE__);
    }
    {    
        string expected = "Employed";
        string result = db.Last({2012,7,15});
        AssertEqual(result,expected,"Last return wrong value",__LINE__,__FILE__);
    }
    {    
        string expected = "SchoolEnd";
        string result = db.Last({2012,7,14});
        AssertEqual(result,expected,"Last return wrong value",__LINE__,__FILE__);
    }
    {    
        string expected = "New-Year";
        string result = db.Last({2002,7,16});
        AssertEqual(result,expected,"Last return wrong value",__LINE__,__FILE__);
    }
    db.Print(cout);
}

int main(void) {
    TestRunner tr;
    tr.RunTest(TestDatabase,"TestDatabase");
    return 0;
}
#endif