#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <tuple>
#include <array>
#include <numeric>
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
};

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
    static constexpr double TAX_RATE = 0.87;

    map<Date,double> date_to_balance_;
#ifdef _DEBUG
public:
#endif
    auto GetDatesRange(const Date& from, const Date& to) {
        bool from_exists = date_to_balance_.count(from);
        bool to_exists = date_to_balance_.count(to);
        // Both found, return the existing range
        if (from_exists && to_exists) {
            auto it_from = date_to_balance_.lower_bound(from);
            auto it_to = date_to_balance_.upper_bound(to);
            return Range(it_from,it_to);
        // 'from' not found, fill in every date from 'to' down to 'from'
        } else if (!from_exists && to_exists) {
            auto it_to = date_to_balance_.upper_bound(to);
            Date insertion{to};
            // TODO: check including ranges
            while(insertion >= from) {
                date_to_balance_[--insertion];
            }
            return Range(date_to_balance_.find(from),it_to);
        // 'to' not found, fill in every date from 'from' to 'to'
        } else if (from_exists && !to_exists) {
            auto it_from = date_to_balance_.lower_bound(from);
            Date insertion{from};
            while(insertion < to) {
                date_to_balance_[++insertion];
            }
            return Range(it_from,++date_to_balance_.find(to));
        // Neither found
        } else {
            Date insertion{from};
            while(insertion <= to) {
                date_to_balance_[insertion++];
            }
            return Range(date_to_balance_.find(from),++date_to_balance_.find(to));
        }
    }
public:
    BudgetManager() {}
    void ComputeIncome(const Date& from, const Date& to, ostream& os = cout) const {
        auto it_from = date_to_balance_.lower_bound(from);
        auto it_to = date_to_balance_.upper_bound(to);
        
        double res  = accumulate(it_from, it_to, .0, [](const double& acc, const pair<Date,double>& rhs) {
            return acc + rhs.second;
        });
        os << res << '\n';
    }
    void Earn(const Date& from, const Date& to, double value) {
        Range range = GetDatesRange(from,to);
        double daily_income = value / range.size();
        for(auto& p : range) {
            p.second += daily_income;
        }
    }
    void PayTax(const Date& from, const Date& to) {
        Range range = GetDatesRange(from,to);
        for(auto& p : range) {
            p.second *= TAX_RATE;
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
    }
}

void TestGetDatesRange() {
    BudgetManager bm;

    DoRangeCheck(bm, 
                Date{.year = 1991, .month = JAN, .day = 10},
                Date{.year = 1991, .month = FEB, .day = 1}
    );

    DoRangeCheck(bm, 
                Date{.year = 1991, .month = JAN, .day = 10},
                Date{.year = 1991, .month = FEB, .day = 1}
    );

    DoRangeCheck(bm, 
                Date{.year = 1991, .month = JAN, .day = 15},
                Date{.year = 1991, .month = JAN, .day = 23}
    );

    DoRangeCheck(bm, 
                Date{.year = 1991, .month = JAN, .day = 15},
                Date{.year = 1991, .month = FEB, .day = 23}
    );

    DoRangeCheck(bm, 
                Date{.year = 1990, .month = AUG, .day = 4},
                Date{.year = 1991, .month = FEB, .day = 23}
    );

}

void TestBudgetManager() {
    {
        BudgetManager bm;
        bm.Earn(Date{.year = 1991, .month = JAN, .day = 1},
                    Date{.year = 1991, .month = JAN, .day = 31}, 31);
        ostringstream os;
        bm.ComputeIncome(Date{.year = 1991, .month = JAN, .day = 1},
                    Date{.year = 1991, .month = JAN, .day = 15}, os);
        ASSERT_EQUAL("15\n",os.str());
    }
    {
        BudgetManager bm;
        bm.Earn(Date{.year = 1991, .month = JAN, .day = 1},
                    Date{.year = 1991, .month = JAN, .day = 1}, 1);
        bm.Earn(Date{.year = 1991, .month = JAN, .day = 3},
                    Date{.year = 1991, .month = JAN, .day = 4}, 2);
        bm.Earn(Date{.year = 1991, .month = JAN, .day = 6},
                    Date{.year = 1991, .month = JAN, .day = 6}, 1);
        ostringstream os;
        bm.ComputeIncome(Date{.year = 1991, .month = JAN, .day = 1},
                    Date{.year = 1991, .month = JAN, .day = 6}, os);
        ASSERT_EQUAL("4\n",os.str());
    }
    {
        BudgetManager bm;
        ostringstream os;
        bm.Earn(Date{.year = 1991, .month = FEB, .day = 1},
                    Date{.year = 1991, .month = MAR, .day = 1}, 29);
        bm.ComputeIncome(Date{.year = 1991, .month = FEB, .day = 1},
                    Date{.year = 1991, .month = MAR, .day = 1}, os);
        ASSERT_EQUAL("29\n",os.str());
        bm.PayTax(Date{.year = 1991, .month = FEB, .day = 1},
                    Date{.year = 1991, .month = MAR, .day = 1});
        os.str("");
        bm.ComputeIncome(Date{.year = 1991, .month = FEB, .day = 1},
                    Date{.year = 1991, .month = MAR, .day = 1}, os);
        ASSERT_EQUAL("25.23\n",os.str());

        bm.Earn(Date{.year = 1991, .month = FEB, .day = 28},
                    Date{.year = 1991, .month = FEB, .day = 29}, 2);
        os.str("");
        bm.ComputeIncome(Date{.year = 1991, .month = FEB, .day = 1},
                    Date{.year = 1991, .month = MAR, .day = 1}, os);
        ASSERT_EQUAL("27.23\n",os.str());

        bm.Earn(Date{.year = 1991, .month = JAN, .day = 1},
                    Date{.year = 1991, .month = JAN, .day = 31}, 31);
        bm.PayTax(Date{.year = 1991, .month = JAN, .day = 1},
                    Date{.year = 1991, .month = MAR, .day = 1});
        
        os.str("");
        bm.ComputeIncome(Date{.year = 1991, .month = JAN, .day = 1},
                    Date{.year = 1991, .month = MAR, .day = 1}, os);
        ASSERT_EQUAL("50.6601\n",os.str());
        
    }
    {
        BudgetManager bm;
        ostringstream os;
        bm.Earn(Date{.year = 1991, .month = JUL, .day = 1},
                    Date{.year = 1991, .month = JUL, .day = 31}, 31);
        bm.Earn(Date{.year = 1991, .month = OCT, .day = 1},
                    Date{.year = 1991, .month = NOV, .day = 15}, 46);

        bm.PayTax(Date{.year = 1991, .month = JUN, .day = 1},
                    Date{.year = 1991, .month = JUL, .day = 31});
        
        bm.ComputeIncome(Date{.year = 1991, .month = JUL, .day = 15},
                    Date{.year = 1991, .month = NOV, .day = 1}, os);
        
        ASSERT_EQUAL("46.79\n",os.str());
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
        ostringstream os;
        bm.Earn(Date{.year = 2000, .month = JAN, .day = 2},
                    Date{.year = 2000, .month = JAN, .day = 6}, 20);
        bm.ComputeIncome(Date{.year = 2000, .month = JAN, .day = 1},
                    Date{.year = 2001, .month = JAN, .day = 1}, os);
        ASSERT_EQUAL("20\n",os.str());
        os.str("");
        bm.PayTax(Date{.year = 2000, .month = JAN, .day = 2},
                    Date{.year = 2000, .month = JAN, .day = 3});
        bm.ComputeIncome(Date{.year = 2000, .month = JAN, .day = 1},
                    Date{.year = 2001, .month = JAN, .day = 1}, os);
        ASSERT_EQUAL("18.96\n",os.str());
        os.str("");
        bm.Earn(Date{.year = 2000, .month = JAN, .day = 3},
                    Date{.year = 2000, .month = JAN, .day = 3}, 10);
        bm.ComputeIncome(Date{.year = 2000, .month = JAN, .day = 1},
                    Date{.year = 2001, .month = JAN, .day = 1}, os);
        ASSERT_EQUAL("28.96\n",os.str());
        os.str("");
        bm.PayTax(Date{.year = 2000, .month = JAN, .day = 3},
                    Date{.year = 2000, .month = JAN, .day = 3});
        bm.ComputeIncome(Date{.year = 2000, .month = JAN, .day = 1},
                    Date{.year = 2001, .month = JAN, .day = 1}, os);
        ASSERT_EQUAL("27.2076\n",os.str());
        os.str("");
    }
}
#endif

int main(void) {
#ifdef _DEBUG
    TestRunner tr;
    RUN_TEST(tr,TestDateOperators);
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
            int income;
            cin >> from >> to >> income;
            bm.Earn(from,to,income);
        } else if (cmd == "PayTax") {
            Date from, to;
            cin >> from >> to;
            bm.PayTax(from,to);
        } else if (cmd == "ComputeIncome") {
            Date from, to;
            cin >> from >> to;
            bm.ComputeIncome(from,to);
        } else {
            throw runtime_error("Unknown command");
        }
    }
    return 0;
}