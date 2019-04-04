#include "database.h"
#include <iterator>
#include <algorithm>
#include <iostream>
#include <sstream>
#include "condition_parser.h"
#include "test_framework.h"

void Database::Add(Date date, const string& event) {
    if(db.count(date)) {
        auto events = db.at(date);
        auto it = find_if(events.begin(), events.end(),[&event](const string& s) {
            return (s == event);
        });
        if (it == events.end()) {
            db[date].push_back(event);
        }
    } else {
        db[date].push_back(event);
    }
}

void Database::Print(ostream& os) const {
    for(const auto& date : db) {
        for(const auto& event : date.second) {
        os << date.first << " " << event << endl;
        }
    }
}

string Database::Last(Date date) {
    if (db.empty() || date < db.begin()->first) {
        throw invalid_argument("No entries");
    }
    auto it = upper_bound(db.begin(),db.end(),date,[](const Date &a, const pair<Date,deque<string>> &b) {
         return a < b.first; 
    });
    if (it != db.begin()) {
        it = prev(it);
    }
    ostringstream ss;
    ss << it->first << " " << *prev(it->second.end());
    return ss.str();
}

void TestDatabase(void) {
    Database db;
    {
        string expected = "No entries";
        try {
            string result = db.Last({1995,5,1});
        } catch (invalid_argument& res) {
            AssertEqual(res.what(),expected,"Last return wrong value",__LINE__,__FILE__);
        }
    }
    db.Add(Date{1990,6,21},"Birthday");
    db.Add(Date{1997,9,1},"School");
    db.Add(Date{1997,9,1},"End of summer");
    db.Add(Date{2001,1,1},"New-Year");
    db.Add(Date{2001,1,1},"Millenium");
    db.Add(Date{2001,1,1},"New-Year");
    db.Add(Date{2007,7,1},"SchoolEnd");
    db.Add(Date{2012,7,15},"Employed");
    {    
        string expected = "No entries";
        try {
            string result = db.Last({1990,6,20});
        } catch (invalid_argument& res) {
            AssertEqual(res.what(),expected,"Last return wrong value",__LINE__,__FILE__);
        }
    }
    {    
        string expected = "1990-06-21 Birthday";
        string result = db.Last({1990,6,21});
        AssertEqual(result,expected,"Last return wrong value",__LINE__,__FILE__);
    }
    {    
        string expected = "1990-06-21 Birthday";
        string result = db.Last({1995,5,1});
        AssertEqual(result,expected,"Last return wrong value",__LINE__,__FILE__);
    }
    {    
        string expected = "2007-07-01 SchoolEnd";
        string result = db.Last({2008,1,1});
        AssertEqual(result,expected,"Last return wrong value",__LINE__,__FILE__);
    }
    {    
        string expected = "2012-07-15 Employed";
        string result = db.Last({2012,7,16});
        AssertEqual(result,expected,"Last return wrong value",__LINE__,__FILE__);
    }
    {    
        string expected = "2012-07-15 Employed";
        string result = db.Last({2012,7,15});
        AssertEqual(result,expected,"Last return wrong value",__LINE__,__FILE__);
    }
    {    
        string expected = "2007-07-01 SchoolEnd";
        string result = db.Last({2012,7,14});
        AssertEqual(result,expected,"Last return wrong value",__LINE__,__FILE__);
    }
    {    
        string expected = "2001-01-01 Millenium";
        string result = db.Last({2002,7,16});
        AssertEqual(result,expected,"Last return wrong value",__LINE__,__FILE__);
    }
    {    
        string expected = "1997-09-01 End of summer";
        string result = db.Last({1997,9,1});
        AssertEqual(result,expected,"Last return wrong value",__LINE__,__FILE__);
    }
    {
        string expected = "1990-06-21 Birthday\n1997-09-01 School\n1997-09-01 End of summer\n2001-01-01 New-Year\n2001-01-01 Millenium\n2007-07-01 SchoolEnd\n2012-07-15 Employed\n";
        ostringstream os;
        db.Print(os);
        string result = os.str();
        AssertEqual(result,expected,"Print falied",__LINE__,__FILE__);
    }
    {
        string expected = "1997-09-01 School\n1997-09-01 End of summer\n2001-01-01 New-Year\n2001-01-01 Millenium\n2007-07-01 SchoolEnd\n2012-07-15 Employed\n";
        istringstream is("date < 1995-01-01");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date& date, const string& event) {
            return condition->Evaluate(date, event);
        };
        int count = db.RemoveIf(predicate);
        ostringstream os;
        db.Print(os);
        string result = os.str();
        AssertEqual(result,expected,"RemoveIf falied",__LINE__,__FILE__);
    }
    {
        vector<Entry> expected = {{{1997,9,1},"School"},{{2001,1,1},"New-Year"},{{2001,1,1},"Millenium"},{{2007,7,1},"SchoolEnd"},{{2012,7,15},"Employed"}};
        istringstream is(R"(event != "End of summer")");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date& date, const string& event) {
            return condition->Evaluate(date, event);
        };
        vector<Entry> result = db.FindIf(predicate);
        AssertEqual(result,expected,"FindIf falied",__LINE__,__FILE__);
    }
    {
        Database db1;
        db1.Add(Date{2017,11,21},"Tuesday");
        db1.Add(Date{2017,11,20},"Monday");
        db1.Add(Date{2017,11,21},"Weekly meeting");
        vector<Entry> expected = {{{2017,11,20},"Monday"},{{2017,11,21},"Tuesday"}};
        istringstream is(R"(event != "Weekly meeting")");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date& date, const string& event) {
            return condition->Evaluate(date, event);
        };
        vector<Entry> result = db1.FindIf(predicate);
        AssertEqual(result,expected,"FindIf falied",__LINE__,__FILE__);
    }
}