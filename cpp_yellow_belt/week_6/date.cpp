#include "date.h"
#include <vector>
#include <iomanip>
#include <sstream>
#include "test_runner.h"

using namespace std;

Date::Date(int year, int month, int day) : year_(year), month_(month), day_(day) {
        if (year_ < 0 || year_ > 9999 || month_ <= 0 || 
            month_ > 12 || day_ <= 0 || day > 31) {
            throw invalid_argument("Incorrect date");
        }
    }

ostream& operator<<(ostream& os, const Date& d) {
    os << setw(4) << setfill('0') << d.GetYear() << "-" << setw(2) << d.GetMonth() << "-" << setw(2) << d.GetDay();
    return os;
}

bool operator<(const Date& lhs, const Date& rhs) {
    vector<int> lhs_ = {lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()};
    vector<int> rhs_ = {rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
    return (lhs_ < rhs_);
}

bool operator!=(const Date& lhs, const Date& rhs) {
    vector<int> lhs_ = {lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()};
    vector<int> rhs_ = {rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
    return (lhs_ != rhs_);
}

bool operator>(const Date& lhs, const Date& rhs) {
    vector<int> lhs_ = {lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()};
    vector<int> rhs_ = {rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
    return (lhs_ > rhs_);
}
bool operator<=(const Date& lhs, const Date& rhs) {
    vector<int> lhs_ = {lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()};
    vector<int> rhs_ = {rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
    return (lhs_ <= rhs_);
}
bool operator>=(const Date& lhs, const Date& rhs){
    vector<int> lhs_ = {lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()};
    vector<int> rhs_ = {rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
    return (lhs_ >= rhs_);
}
bool operator==(const Date& lhs, const Date& rhs){
    vector<int> lhs_ = {lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()};
    vector<int> rhs_ = {rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
    return (lhs_ == rhs_);
}

Date ParseDate(istream& is) {
    int year,month,day;
    char temp1,temp2;
    is >> year >> temp1 >> month >> temp2 >> day;
    if (temp1 == '-' && temp2 == '-') {
        return Date(year,month,day);
    } else {
        throw invalid_argument("Wrong date format");
    }
}

void TestParseDate() {
    {
        istringstream ss("0-1-1");
        Date expected(0,1,1);
        AssertEqual(ParseDate(ss),expected,"Date error");
    }
    {
        istringstream ss("1990-1-1");
        Date expected(1990,1,1);
        Date result = ParseDate(ss);
        AssertEqual(result,expected,"Date error");
    }
    {
        istringstream ss("10000-1-1");
        try {    
            Date result = ParseDate(ss);
        } catch(invalid_argument& e) {
            string expected = "Incorrect date";
            AssertEqual(e.what(),expected,"Date error");
        }
    }
    {
        istringstream ss("100-13-1");
        try {    
            Date result = ParseDate(ss);
        } catch(invalid_argument& e) {
            string expected = "Incorrect date";
            AssertEqual(e.what(),expected,"Date error");
        }
    }
    {
        istringstream ss("100-1-32");
        try {    
            Date result = ParseDate(ss);
        } catch(invalid_argument& e) {
            string expected = "Incorrect date";
            AssertEqual(e.what(),expected,"Date error");
        }
    }
    {
        istringstream ss("0--1-1");
        try {    
            Date result = ParseDate(ss);
        } catch(invalid_argument& e) {
            string expected = "Incorrect date";
            AssertEqual(e.what(),expected,"Date error");
        }
    }
    {
        istringstream ss("-");
        try {    
            Date result = ParseDate(ss);
        } catch(invalid_argument& e) {
            string expected = "Wrong date format";
            AssertEqual(e.what(),expected,"Date error");
        }
    }
    {
        istringstream ss("1990+06-9");
        try {    
            Date result = ParseDate(ss);
        } catch(invalid_argument& e) {
            string expected = "Wrong date format";
            AssertEqual(e.what(),expected,"Date error");
        }
    }
}

void TestDataComparison(void) {
    {
        istringstream ss1("0-1-1");
        istringstream ss2("1-1-1");
        Date d1 = ParseDate(ss1);
        Date d2 = ParseDate(ss2);
        AssertEqual(d1<=d2,true,"Date error");
        AssertEqual(d1>=d2,false,"Date error");
        AssertEqual(d1==d2,false,"Date error");
        AssertEqual(d1<d2,true,"Date error");
        AssertEqual(d1>d2,false,"Date error");
    }
    {
        istringstream ss1("1-1-1");
        istringstream ss2("1-1-1");
        Date d1 = ParseDate(ss1);
        Date d2 = ParseDate(ss2);
        AssertEqual(d1<=d2,true,"Date error");
        AssertEqual(d1>=d2,true,"Date error");
        AssertEqual(d1==d2,true,"Date error");
        AssertEqual(d1<d2,false,"Date error");
        AssertEqual(d1>d2,false,"Date error");
    }
    {
        Assert(  Date{2017, 1, 1} == Date{2017, 1, 1},  "==");
        Assert(!(Date{2017, 1, 1} == Date{2017, 1, 2}), "!==");
        Assert(!(Date{2017, 1, 1} == Date{2017, 2, 1}), "!==");
        Assert(!(Date{2017, 1, 1} == Date{2018, 1, 1}), "!==");
        Assert(!(Date{2017, 1, 2} == Date{2017, 1, 1}), "!==");
        Assert(!(Date{2017, 2, 1} == Date{2017, 1, 1}), "!==");
        Assert(!(Date{2018, 1, 1} == Date{2017, 1, 1}), "!==");

        Assert(  Date{2017, 1, 1} != Date{2017, 1, 2},  "!=");
        Assert(  Date{2017, 1, 1} != Date{2017, 2, 1},  "!=");
        Assert(  Date{2017, 1, 1} != Date{2018, 1, 1},  "!=");
        Assert(  Date{2017, 1, 2} != Date{2017, 1, 1},  "!=");
        Assert(  Date{2017, 2, 1} != Date{2017, 1, 1},  "!=");
        Assert(  Date{2018, 1, 1} != Date{2017, 1, 1},  "!=");
        Assert(!(Date{2017, 1, 1} != Date{2017, 1, 1}), "!!=");

        Assert(  Date{2017, 1, 1} <= Date{2017, 1, 1},  "<=");
        Assert(  Date{2017, 1, 1} <= Date{2017, 1, 2},  "<=");
        Assert(  Date{2017, 1, 1} <= Date{2017, 2, 1},  "<=");
        Assert(  Date{2017, 1, 1} <= Date{2018, 1, 1},  "<=");
        Assert(!(Date{2017, 1, 2} <= Date{2017, 1, 1}), "!<=");
        Assert(!(Date{2017, 2, 1} <= Date{2017, 1, 1}), "!<=");
        Assert(!(Date{2018, 1, 1} <= Date{2017, 1, 1}), "!<=");

        Assert(  Date{2017, 1, 1} <  Date{2017, 1, 2},  "<");
        Assert(  Date{2017, 1, 1} <  Date{2017, 2, 1},  "<");
        Assert(  Date{2017, 1, 1} <  Date{2018, 1, 1},  "<");
        Assert(!(Date{2017, 1, 1} <  Date{2017, 1, 1}), "!<");
        Assert(!(Date{2017, 1, 2} <  Date{2017, 1, 1}), "!<");
        Assert(!(Date{2017, 2, 1} <  Date{2017, 1, 1}), "!<");
        Assert(!(Date{2018, 1, 1} <  Date{2017, 1, 1}), "!<");

        Assert(  Date{2017, 1, 1} >= Date{2017, 1, 1} , ">=");
        Assert(  Date{2017, 1, 2} >= Date{2017, 1, 1} , ">=");
        Assert(  Date{2017, 2, 1} >= Date{2017, 1, 1} , ">=");
        Assert(  Date{2018, 1, 1} >= Date{2017, 1, 1} , ">=");
        Assert(!(Date{2017, 1, 1} >= Date{2017, 1, 2}), "!>=");
        Assert(!(Date{2017, 1, 1} >= Date{2017, 2, 1}), "!>=");
        Assert(!(Date{2017, 1, 1} >= Date{2018, 1, 1}), "!>=");

        Assert(  Date{2017, 1, 2} >  Date{2017, 1, 1} , ">");
        Assert(  Date{2017, 2, 1} >  Date{2017, 1, 1} , ">");
        Assert(  Date{2018, 1, 1} >  Date{2017, 1, 1} , ">");
        Assert(!(Date{2017, 1, 1} >  Date{2017, 1, 1}), "!>");
        Assert(!(Date{2017, 1, 1} >  Date{2017, 1, 2}), "!>");
        Assert(!(Date{2017, 1, 1} >  Date{2017, 2, 1}), "!>");
        Assert(!(Date{2017, 1, 1} >  Date{2018, 1, 1}), "!>");
    }
}