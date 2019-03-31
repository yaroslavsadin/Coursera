#pragma once
#include <iostream>

using namespace std;

class Date {
public:
    Date(int year, int month, int day) : year_(year), month_(month), day_(day) {
        if (year_ < 0 || year_ > 9999 || month_ <= 0 || 
            month_ > 12 || day_ <= 0 || day > 31) {
            throw invalid_argument("Incorrect date");
        }
    }
    inline int GetYear() const {return year_;}
    inline int GetMonth() const {return month_;}
    inline int GetDay() const {return day_;}
private:
    int year_;
    int month_;
    int day_;
};

ostream& operator<<(ostream& os, const Date& d);
bool operator<(const Date& lhs, const Date& rhs);
Date ParseDate(const istream& is);