#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <tuple>
#include <array>
#include <numeric>
#include <ctime>
#include "test_runner.h"

using namespace std;

template<typename ItT>
class Range {
public:
    Range(ItT begin, ItT end) :
    begin_(begin), end_(end), size_(std::distance(begin_,end_)) {}
    ItT begin() const { return begin_; }
    ItT end() const { return end_; }
    size_t size() { return size_; }
private:
    ItT begin_;
    ItT end_;
    size_t size_;
};

struct Date {

#define JAN 1
#define FEB 2
#define MAR 3
#define APR 4
#define MAY 5
#define JUN 6
#define JUL 7
#define AUG 8
#define SEP 9
#define OCT 10
#define NOV 11
#define DEC 12

    int year; int month; int day;
    const static array<int,13> month_days_count;

#define DATE_COMP_OP(op)                                                            \
    friend bool operator op (const Date& lhs, const Date& rhs) {                    \
        return tie(lhs.year,lhs.month,lhs.day) op tie(rhs.year,rhs.month, rhs.day); \
    }
    DATE_COMP_OP(>);
    DATE_COMP_OP(<);
    DATE_COMP_OP(>=);
    DATE_COMP_OP(<=);
    DATE_COMP_OP(==);
    DATE_COMP_OP(!=);
#undef DATE_COMP_OP

    friend istream& operator>>(istream& is, Date& date) {
        char dash;
        int year, month, day;
        is >> year >> dash >> month >> dash >> day;
        date = {year,month,day};
        return is;
    }

    friend ostream& operator<<(ostream& os, Date& date) {
        return os << "Date{.year = " << date.year << ", .month = " << date.month << ", .day = " << date.day << "}";
    }

    Date& operator++ ()     // prefix ++
    {
        // Do work on this.   (increment your object here)
        if(++day > GetMonthDays()) {
            day = 1;
            if(++month > 12) {
                ++year;
                month = JAN;
            }
        }
        return *this;
    }

    Date  operator++ (int)  // postfix ++
    {
        Date result(*this);   // make a copy for result
        ++(*this);              // Now use the prefix version to do the work
        return result;          // return the copy (the old) value.
    }

    Date& operator-- ()     // prefix --
    {
        // Do work on this.   (decrement your object here)
        if(--day < 1) {
            if(--month < 1) {
                month = DEC;
                --year;
            }
            day = GetMonthDays();
        }
        return *this;
    }

    Date  operator-- (int)  // postfix --
    {
        Date result(*this);   // make a copy for result
        --(*this);              // Now use the prefix version to do the work
        return result;          // return the copy (the old) value.
    }

    bool IsLeapYear() {
        bool res = false;
        if (year%400 == 0) // Exactly divisible by 400 e.g. 1600, 2000
            res = true;
        else if (year%100 == 0) // Exactly divisible by 100 and not by 400 e.g. 1900, 2100
            res = false;
        else if (year%4 == 0) // Exactly divisible by 4 and neither by 100 nor 400 e.g. 2016, 2020
            res = true;
        else // Not divisible by 4 or 100 or 400 e.g. 2017, 2018, 2019
            res = false;
        return res;
    }

    int GetMonthDays() {
        int days_in_month = month_days_count[month];
        if(month == FEB && IsLeapYear()) {
            days_in_month++;
        }
        return days_in_month;
    }

    time_t AsTimestamp() const {
        std::tm t;
        t.tm_sec   = 0;
        t.tm_min   = 0;
        t.tm_hour  = 0;
        t.tm_mday  = day;
        t.tm_mon   = month - 1;
        t.tm_year  = year - 1900;
        t.tm_isdst = 0;
        return mktime(&t);
    }
};

int ComputeDaysDiff(const Date& date_to, const Date& date_from) {
  const time_t timestamp_to = date_to.AsTimestamp();
  const time_t timestamp_from = date_from.AsTimestamp();
  static const int SECONDS_IN_DAY = 60 * 60 * 24;
  return (timestamp_to - timestamp_from) / SECONDS_IN_DAY;
}

const array<int,13> Date::month_days_count = {
    0,  // padding
    31,
    28, // Leap year is checked later
    31,
    30,
    31,
    30,
    31,
    31,
    30,
    31,
    30,
    31
};

class BudgetManager {
protected:
private:
    struct Balance {
        double income = 0.0;
        double spending = 0.0;
    };

    using Storage = map<Date,Balance>;

    Storage date_to_balance_;
    
#ifdef _DEBUG
public:
#endif
    auto GetDatesRange(const Date& from, const Date& to) {
        // bool from_exists = date_to_balance_.count(from);
        // bool to_exists = date_to_balance_.count(to);

        if (true) {
            Date insertion{from};
            while(insertion <= to) {
                date_to_balance_[insertion++];
            }
        }

        auto it_from = date_to_balance_.lower_bound(from);
        auto it_to = date_to_balance_.upper_bound(to);
        return Range(it_from,it_to);
    }
public:
    BudgetManager() {}
    size_t size() { return date_to_balance_.size(); }
    double ComputeIncome(const Date& from, const Date& to) const {
        auto it_from = date_to_balance_.lower_bound(from);
        auto it_to = date_to_balance_.upper_bound(to);

        double res  = accumulate(it_from, it_to, .0, [](const double acc, const Storage::value_type& rhs) {
            return acc + (rhs.second.income - rhs.second.spending);
        });
        return res;
    }
    void Earn(const Date& from, const Date& to, double value) {
        Range range = GetDatesRange(from,to);
        double daily_income = value / (ComputeDaysDiff(to, from) + 1);
        for(auto& p : range) {
            p.second.income += daily_income;
        }
    }
    void Spend(const Date& from, const Date& to, double value) {
        Range range = GetDatesRange(from,to);
        double daily_spending = value / (ComputeDaysDiff(to, from) + 1);
        for(auto& p : range) {
            p.second.spending += daily_spending;
        }
    }
    void PayTax(const Date& from, const Date& to, unsigned int percentage = 13) {
        Range range = GetDatesRange(from,to);
        double tax_rate = (100 - percentage) / 100.;
        for(auto& p : range) {
            p.second.income *= tax_rate;
        }
    }
};

#ifdef _DEBUG
void TestDateOperators() {
    Date bigger{.year = 1990, .month = JUN, .day = 21};
    Date smaller{.year = 1990, .month = JUN, .day = 20};
    ASSERT(smaller < bigger);
    ASSERT(bigger > smaller);
    ASSERT(bigger >= smaller);
    ASSERT(smaller <= bigger);
    ASSERT(smaller != bigger);
    bigger = {.year = 1990, .month = JUN, .day = 20};
    ASSERT(smaller == bigger);
    ASSERT(smaller == bigger);
}

void TestDateIncDec() {
    {
        Date result{.year = 1990, .month = DEC, .day = 30};
        Date expected{.year = 1990, .month = DEC, .day = 31};
        ASSERT(++result == expected);
    }
    {
        Date result{.year = 2016, .month = MAR, .day = 1};
        Date expected{.year = 2016, .month = FEB, .day = 29};
        ASSERT(--result == expected);
    }
    {
        Date result{.year = 2019, .month = MAR, .day = 1};
        Date expected{.year = 2019, .month = FEB, .day = 28};
        ASSERT(--result == expected);
    }
    {
        Date result{.year = 2020, .month = FEB, .day = 28};
        Date expected{.year = 2020, .month = FEB, .day = 29};
        ASSERT(++result == expected);
    }
    {
        Date result{.year = 2020, .month = FEB, .day = 29};
        Date expected{.year = 2020, .month = MAR, .day = 1};
        ASSERT(++result == expected);
    }
    {
        Date result{.year = 2020, .month = DEC, .day = 31};
        Date expected{.year = 2021, .month = JAN, .day = 1};
        ASSERT(++result == expected);
    }
    {
        Date result{.year = 2021, .month = JAN, .day = 1};
        Date expected{.year = 2020, .month = DEC, .day = 31};
        ASSERT(--result == expected);
    }
}

void TestDatesDiff() {
    {
        Date from{.year = 2020, .month = JAN, .day = 31};
        Date to{.year = 2020, .month = JAN, .day = 31};
        ASSERT_EQUAL(0,ComputeDaysDiff(to,from));
    }
    {
        Date from{.year = 2020, .month = JAN, .day = 1};
        Date to{.year = 2020, .month = JAN, .day = 2};
        ASSERT_EQUAL(1,ComputeDaysDiff(to,from));
    }
    {
        Date from{.year = 2020, .month = JAN, .day = 1};
        Date to{.year = 2020, .month = JAN, .day = 15};
        ASSERT_EQUAL(14,ComputeDaysDiff(to,from));
    }
}

void DoRangeCheck(BudgetManager& bm, const Date& from, const Date& to) {
    vector<Date> expected_ = [from,to] {
        vector<Date> v;
        for(Date i = from; i <= to; i++) {
            v.push_back(i);
        }
        return v;
    }();
    Range expected(expected_.begin(),expected_.end());

    Range result = bm.GetDatesRange(from,to);
    ASSERT_EQUAL(expected.size(),result.size());
    auto it = expected.begin();
    for(const auto& d : result) {
        ASSERT(d.first == *it++);
        ASSERT(d.second.income == 0.0);
        ASSERT(d.second.spending == 0.0);
    }
}

void TestGetDatesRange() {
    {
        BudgetManager bm;

        // Both missing
        // 23
        DoRangeCheck(bm, 
                    Date{.year = 1991, .month = JAN, .day = 10},
                    Date{.year = 1991, .month = FEB, .day = 1}
        );
        ASSERT_EQUAL(bm.size(),23u);

        // From exists, To exists
        // 23 
        DoRangeCheck(bm, 
                    Date{.year = 1991, .month = JAN, .day = 10},
                    Date{.year = 1991, .month = FEB, .day = 1}
        );
        ASSERT_EQUAL(bm.size(),23u);

        // From exists, To exists
        DoRangeCheck(bm, 
                    Date{.year = 1991, .month = JAN, .day = 15},
                    Date{.year = 1991, .month = JAN, .day = 23}
        );
        ASSERT_EQUAL(bm.size(),23u);

        // From exists, To does not
        // 46
        DoRangeCheck(bm, 
                    Date{.year = 1991, .month = JAN, .day = 15},
                    Date{.year = 1991, .month = FEB, .day = 23}
        );
        ASSERT_EQUAL(bm.size(),45u);

        // From does not exist, To does
        // 204
        DoRangeCheck(bm, 
                    Date{.year = 1990, .month = AUG, .day = 4},
                    Date{.year = 1991, .month = FEB, .day = 15}
        );
        ASSERT_EQUAL(bm.size(),204u);

        // From does not exist, To does
        // 206
        DoRangeCheck(bm, 
                    Date{.year = 1991, .month = FEB, .day = 27},
                    Date{.year = 1991, .month = FEB, .day = 28}
        );
        ASSERT_EQUAL(bm.size(),206u);

        // From does not exist, To does
        // 207
        DoRangeCheck(bm, 
                    Date{.year = 1991, .month = MAR, .day = 1},
                    Date{.year = 1991, .month = MAR, .day = 1}
        );
        ASSERT_EQUAL(bm.size(),207u);

        // Both missing
        // 217
        DoRangeCheck(bm, 
                    Date{.year = 2016, .month = MAR, .day = 1},
                    Date{.year = 2016, .month = MAR, .day = 10}
        );
        ASSERT_EQUAL(bm.size(),217u);

        // From does not exist, To does
        // 246
        DoRangeCheck(bm, 
                    Date{.year = 2016, .month = FEB, .day = 1},
                    Date{.year = 2016, .month = MAR, .day = 1}
        );
        ASSERT_EQUAL(bm.size(),246u);

        // From does not exist, To does
        // 246
        DoRangeCheck(bm, 
                    Date{.year = 2016, .month = FEB, .day = 1},
                    Date{.year = 2016, .month = MAR, .day = 1}
        );
        ASSERT_EQUAL(bm.size(),246u);
    }

    {
        BudgetManager bm;

        // Both missing
        // 30
        DoRangeCheck(bm, 
                    Date{.year = 2016, .month = FEB, .day = 1},
                    Date{.year = 2016, .month = MAR, .day = 1}
        );
        ASSERT_EQUAL(bm.size(),30u);

        // Both missing
        // 59
        DoRangeCheck(bm, 
                    Date{.year = 2017, .month = FEB, .day = 1},
                    Date{.year = 2017, .month = MAR, .day = 1}
        );
        ASSERT_EQUAL(bm.size(),59u);
    }
}

void TestBudgetManager() {
    {
        BudgetManager bm;
        bm.Earn(Date{.year = 1991, .month = JAN, .day = 1},
                    Date{.year = 1991, .month = JAN, .day = 31}, 31);
        
        double result = bm.ComputeIncome(Date{.year = 1991, .month = JAN, .day = 1},
                    Date{.year = 1991, .month = JAN, .day = 15});
        ASSERT_EQUAL(15,result);
    }
    {
        BudgetManager bm;
        bm.Earn(Date{.year = 1991, .month = JAN, .day = 1},
                    Date{.year = 1991, .month = JAN, .day = 1}, 1);
        bm.Earn(Date{.year = 1991, .month = JAN, .day = 3},
                    Date{.year = 1991, .month = JAN, .day = 4}, 2);
        bm.Earn(Date{.year = 1991, .month = JAN, .day = 6},
                    Date{.year = 1991, .month = JAN, .day = 6}, 1);
        bm.Earn(Date{.year = 1991, .month = JAN, .day = 1},
                    Date{.year = 1991, .month = JAN, .day = 6}, 6);
        double result = bm.ComputeIncome(Date{.year = 1991, .month = JAN, .day = 1},
                    Date{.year = 1991, .month = JAN, .day = 6});
        ASSERT_EQUAL(10,result);
    }
    {
        BudgetManager bm;
        bm.Earn(Date{.year = 1991, .month = FEB, .day = 1},
                    Date{.year = 1991, .month = MAR, .day = 1}, 29);
        double result = bm.ComputeIncome(Date{.year = 1991, .month = FEB, .day = 1},
                    Date{.year = 1991, .month = MAR, .day = 1});
        ASSERT_EQUAL(29,result);
        bm.PayTax(Date{.year = 1991, .month = FEB, .day = 1},
                    Date{.year = 1991, .month = MAR, .day = 1});
        result = bm.ComputeIncome(Date{.year = 1991, .month = FEB, .day = 1},
                    Date{.year = 1991, .month = MAR, .day = 1});
        ASSERT_EQUAL(25.230000000000004,result);

        bm.Earn(Date{.year = 1991, .month = FEB, .day = 27},
                    Date{.year = 1991, .month = FEB, .day = 28}, 2);
        result = bm.ComputeIncome(Date{.year = 1991, .month = FEB, .day = 1},
                    Date{.year = 1991, .month = MAR, .day = 1});
        ASSERT_EQUAL(27.230000000000004,result);

        bm.Earn(Date{.year = 1991, .month = JAN, .day = 1},
                    Date{.year = 1991, .month = JAN, .day = 31}, 31);
        bm.PayTax(Date{.year = 1991, .month = JAN, .day = 1},
                    Date{.year = 1991, .month = MAR, .day = 1});
        
        result = bm.ComputeIncome(Date{.year = 1991, .month = JAN, .day = 1},
                    Date{.year = 1991, .month = MAR, .day = 1});
        ASSERT_EQUAL(50.66010000000005,result);
        
    }
    {
        BudgetManager bm;
        
        bm.Earn(Date{.year = 1991, .month = JUL, .day = 1},
                    Date{.year = 1991, .month = JUL, .day = 31}, 31);
        bm.Earn(Date{.year = 1991, .month = OCT, .day = 1},
                    Date{.year = 1991, .month = NOV, .day = 15}, 46);

        bm.PayTax(Date{.year = 1991, .month = JUN, .day = 1},
                    Date{.year = 1991, .month = JUL, .day = 31});
        
        double result = bm.ComputeIncome(Date{.year = 1991, .month = JUL, .day = 15},
                    Date{.year = 1991, .month = NOV, .day = 1});
        
        ASSERT_EQUAL(46.789999999999992,result);
    }
    {
        // 8
        // Earn 2000-01-02 2000-01-06 20
        // ComputeIncome 2000-01-01 2001-01-01
        // PayTax 2000-01-02 2000-01-03
        // ComputeIncome 2000-01-01 2001-01-01
        // Earn 2000-01-03 2000-01-03 10
        // ComputeIncome 2000-01-01 2001-01-01
        // PayTax 2000-01-03 2000-01-03
        // ComputeIncome 2000-01-01 2001-01-01

        BudgetManager bm;
        
        bm.Earn(Date{.year = 2000, .month = JAN, .day = 2},
                    Date{.year = 2000, .month = JAN, .day = 6}, 20);
        double result = bm.ComputeIncome(Date{.year = 2000, .month = JAN, .day = 1},
                    Date{.year = 2001, .month = JAN, .day = 1});
        ASSERT_EQUAL(20,result);
        
        bm.PayTax(Date{.year = 2000, .month = JAN, .day = 2},
                    Date{.year = 2000, .month = JAN, .day = 3});
        result = bm.ComputeIncome(Date{.year = 2000, .month = JAN, .day = 1},
                    Date{.year = 2001, .month = JAN, .day = 1});
        ASSERT_EQUAL(18.96,result);
        
        bm.Earn(Date{.year = 2000, .month = JAN, .day = 3},
                    Date{.year = 2000, .month = JAN, .day = 3}, 10);
        result = bm.ComputeIncome(Date{.year = 2000, .month = JAN, .day = 1},
                    Date{.year = 2001, .month = JAN, .day = 1});
        ASSERT_EQUAL(28.96,result);
        
        bm.PayTax(Date{.year = 2000, .month = JAN, .day = 3},
                    Date{.year = 2000, .month = JAN, .day = 3});
        result = bm.ComputeIncome(Date{.year = 2000, .month = JAN, .day = 1},
                    Date{.year = 2001, .month = JAN, .day = 1});
        ASSERT_EQUAL(27.2076,result);
        
    }
    {
        // 8
        // Earn 2000-01-02 2000-01-06 20
        // ComputeIncome 2000-01-01 2001-01-01
        // PayTax 2000-01-02 2000-01-03 13
        // ComputeIncome 2000-01-01 2001-01-01
        // Spend 2000-12-30 2001-01-02 14
        // ComputeIncome 2000-01-01 2001-01-01
        // PayTax 2000-12-30 2000-12-30 13
        // ComputeIncome 2000-01-01 2001-01-01

        BudgetManager bm;
        
        bm.Earn(Date{.year = 2000, .month = JAN, .day = 2},
                    Date{.year = 2000, .month = JAN, .day = 6}, 20);
        double result = bm.ComputeIncome(Date{.year = 2000, .month = JAN, .day = 1},
                    Date{.year = 2001, .month = JAN, .day = 1});
        ASSERT_EQUAL(20,result);
        
        bm.PayTax(Date{.year = 2000, .month = JAN, .day = 2},
                    Date{.year = 2000, .month = JAN, .day = 3});
        result = bm.ComputeIncome(Date{.year = 2000, .month = JAN, .day = 1},
                    Date{.year = 2001, .month = JAN, .day = 1});
        ASSERT_EQUAL(18.96,result);

        bm.Spend(Date{.year = 2000, .month = DEC, .day = 30},
                    Date{.year = 2001, .month = JAN, .day = 2}, 14);
        result = bm.ComputeIncome(Date{.year = 2000, .month = JAN, .day = 1},
                    Date{.year = 2001, .month = JAN, .day = 1});
        ASSERT_EQUAL(8.46,result);
        
        bm.PayTax(Date{.year = 2000, .month = DEC, .day = 30},
                    Date{.year = 2000, .month = DEC, .day = 30});
        result = bm.ComputeIncome(Date{.year = 2000, .month = JAN, .day = 1},
                    Date{.year = 2001, .month = JAN, .day = 1});
        ASSERT_EQUAL(8.46,result);
    }
    {
        BudgetManager bm;

        double result;
        
        // bm.Earn(Date{.year = 2001, .month = JAN, .day = 1},
        //             Date{.year = 2001, .month = JUL, .day = 31}, 5000000);
        // double result = bm.ComputeIncome(Date{.year = 2000, .month = JAN, .day = 1},
        //             Date{.year = 2001, .month = JAN, .day = 1});
        // ASSERT_EQUAL(23584.9056603773584905660377358,result);

        // result = bm.ComputeIncome(Date{.year = 2001, .month = JUN, .day = 1},
        //             Date{.year = 2001, .month = JUN, .day = 15});
        // ASSERT_EQUAL(353773.5849056603,result);
        
        // bm.Spend(Date{.year = 2001, .month = JUN, .day = 1},
        //             Date{.year = 2001, .month = JUL, .day = 1}, 3000);
        // result = bm.ComputeIncome(Date{.year = 2001, .month = JUN, .day = 1},
        //             Date{.year = 2001, .month = JUN, .day = 1});
        // ASSERT_EQUAL(23488.131466828971393791844187413,result);

        bm.PayTax(Date{.year = 2001, .month = JUN, .day = 1},
                    Date{.year = 2001, .month = JUN, .day = 30});
        result = bm.ComputeIncome(Date{.year = 2000, .month = JUN, .day = 1},
                    Date{.year = 2001, .month = JUN, .day = 30});
        ASSERT_EQUAL(0,result);
    }
}
#endif

int main(void) {
#ifdef _DEBUG
    TestRunner tr;
    RUN_TEST(tr,TestDateOperators);
    RUN_TEST(tr,TestDatesDiff);
    RUN_TEST(tr,TestDateIncDec);
    RUN_TEST(tr,TestGetDatesRange);
    RUN_TEST(tr,TestBudgetManager);
#endif

    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.precision(25);

    BudgetManager bm;
    int num;

    cin >> num;
    while(num--) {
        string cmd;
        cin >> cmd;
        if(cmd == "Earn") {
            Date from, to;
            double income;
            cin >> from >> to >> income;
            bm.Earn(from,to,income);
            // cout << "bm.Earn(" << from <<", " << to << ", " << income << ");" << endl;
        } else if(cmd == "Spend") {
            Date from, to;
            double spending;
            cin >> from >> to >> spending;
            bm.Spend(from,to,spending);
            // cout << "bm.Spend(" << from <<", " << to << ", " << spending << ");" << endl;
        } else if (cmd == "PayTax") {
            Date from, to;
            int percentage;
            cin >> from >> to >> percentage;
            bm.PayTax(from,to,percentage);
        } else if (cmd == "ComputeIncome") {
            Date from, to;
            cin >> from >> to;
            cout << bm.ComputeIncome(from,to) << '\n';
            // cout << "bm.ComputeIncome(" << from <<", " << to << ");" << endl;
        } else {
            throw runtime_error("Unknown command");
        }
    }
    return 0;
}