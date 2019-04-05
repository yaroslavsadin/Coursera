#include "database.h"
#include <iterator>
#include <algorithm>
#include <iostream>
#include <sstream>
#include "condition_parser.h"
#include "test_runner.h"

void Database::Add(const Date& date, const string& event) {
    if(db.count(date)) {
        auto& events = db_.at(date);
        auto found = events.count(event);
        if (!found) {
            db[date].push_back(event);
            db_[date].insert(event);
        }
    } else {
        db[date].push_back(event);
        db_[date].insert(event);
    }
}

void Database::Print(ostream& os) const {
    for(const auto& date : db) {
        for(const auto& event : date.second) {
        os << date.first << " " << event << endl;
        }
    }
}

string Database::Last(const Date& date) const {
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
            AssertEqual(res.what(),expected,"Last return wrong value");
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
            AssertEqual(res.what(),expected,"Last return wrong value");
        }
    }
    {    
        string expected = "1990-06-21 Birthday";
        string result = db.Last({1990,6,21});
        AssertEqual(result,expected,"Last return wrong value");
    }
    {    
        string expected = "1990-06-21 Birthday";
        string result = db.Last({1995,5,1});
        AssertEqual(result,expected,"Last return wrong value");
    }
    {    
        string expected = "2007-07-01 SchoolEnd";
        string result = db.Last({2008,1,1});
        AssertEqual(result,expected,"Last return wrong value");
    }
    {    
        string expected = "2012-07-15 Employed";
        string result = db.Last({2012,7,16});
        AssertEqual(result,expected,"Last return wrong value");
    }
    {    
        string expected = "2012-07-15 Employed";
        string result = db.Last({2012,7,15});
        AssertEqual(result,expected,"Last return wrong value");
    }
    {    
        string expected = "2007-07-01 SchoolEnd";
        string result = db.Last({2012,7,14});
        AssertEqual(result,expected,"Last return wrong value");
    }
    {    
        string expected = "2001-01-01 Millenium";
        string result = db.Last({2002,7,16});
        AssertEqual(result,expected,"Last return wrong value");
    }
    {    
        string expected = "1997-09-01 End of summer";
        string result = db.Last({1997,9,1});
        AssertEqual(result,expected,"Last return wrong value");
    }
    {
        string expected = "1990-06-21 Birthday\n1997-09-01 School\n1997-09-01 End of summer\n2001-01-01 New-Year\n2001-01-01 Millenium\n2007-07-01 SchoolEnd\n2012-07-15 Employed\n";
        ostringstream os;
        db.Print(os);
        string result = os.str();
        AssertEqual(result,expected,"Print falied");
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
        AssertEqual(result,expected,"RemoveIf falied");
    }
    {
        vector<Entry> expected = {{{1997,9,1},"School"},{{2001,1,1},"New-Year"},{{2001,1,1},"Millenium"},{{2007,7,1},"SchoolEnd"},{{2012,7,15},"Employed"}};
        istringstream is(R"(event != "End of summer")");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date& date, const string& event) {
            return condition->Evaluate(date, event);
        };
        vector<Entry> result = db.FindIf(predicate);
        AssertEqual(result,expected,"FindIf falied");
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
        AssertEqual(result,expected,"FindIf falied");
    }
    {
        Database db1;
        db1.Add(Date{2017,11,21},"Tuesday");
        db1.Add(Date{2017,11,20},"Monday");
        db1.Add(Date{2017,11,21},"Weekly meeting");
        string expected = "";
        istringstream is("date > 1995-01-01");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date& date, const string& event) {
            return condition->Evaluate(date, event);
        };
        int count = db1.RemoveIf(predicate);
        ostringstream os;
        db1.Print(os);
        string result = os.str();
        AssertEqual(result,expected,"RemoveIf falied");
    }
    {
        Database db;
        stringstream is {R"(event == "second" OR event == "fourth")"};
        db.Add({2017, 1, 1}, "first");
        db.Add({2017, 1, 1}, "second");
        db.Add({2017, 1, 1}, "third");
        db.Add({2017, 1, 1}, "fourth");
        db.Add({2017, 1, 1}, "five");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date& date, const string& event) {
            return condition->Evaluate(date, event);
        };
        int count = db.RemoveIf(predicate);
        AssertEqual(2, count, "Remove several");
        db.Add({2017, 1, 1}, "second");
        stringstream ss;
        db.Print(ss);
        AssertEqual("2017-01-01 first\n2017-01-01 third\n2017-01-01 five\n2017-01-01 second\n", ss.str(), "Check print after remove several and add the same");

    }
}