#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <tuple>
#include "test_runner.h"

using namespace std;

template<typename ItT>
class Range {
public:
    Range(ItT begin, ItT end) :
    begin_(begin), end_(end) {}
    ItT begin() const { return begin_; }
    ItT end() const { return end_; }
private:
    ItT begin_;
    ItT end_;
};

class BudgetManager {
#ifdef _DEBUG
public:
#endif
    struct Date {
        int year; int month; int day; 
#define DATE_COMP_OP(op)                                                       \
friend bool operator op (const Date& lhs, const Date& rhs) {                    \
    return tie(lhs.year,lhs.month,lhs.day) op tie(rhs.year,rhs.month, rhs.day);  \
}
        DATE_COMP_OP(>);
        DATE_COMP_OP(<);
        DATE_COMP_OP(>=);
        DATE_COMP_OP(<=);
        DATE_COMP_OP(==);
        DATE_COMP_OP(!=);
#undef DATE_COMP_OP

        Date& operator++ ()     // prefix ++
        {
           // Do work on this.   (increment your object here)
           if(++this->day > 31) {
               this->day = 1;
               if(++this->month > 12) {
                   this->month = 1;
                   ++this->year;
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
           if(--this->day < 1) {
               this->day = 31;
               if(--this->month < 1) {
                   this->month = 12;
                   --this->year;
               }
           }
           return *this;
        }

        Date  operator-- (int)  // postfix --
        {
           Date result(*this);   // make a copy for result
           --(*this);              // Now use the prefix version to do the work
           return result;          // return the copy (the old) value.
        }
    };
public:
    BudgetManager() {}
    void ComputeIncome(const Date& from, const Date& to, ostream& os = cout) const;
    void Earn(const Date& from, const Date& to, double value);
    void PayTax(const Date& from, const Date& to);
protected:
private:
    static constexpr double TAX_RATE = 0.13;

    map<Date,double> date_to_balance_;
#ifdef _DEBUG
public:
#endif
    auto GetDatesRange(const Date& from, const Date& to) {
        auto it_from = date_to_balance_.lower_bound(from);
        auto it_to = date_to_balance_.lower_bound(to);

        // Both found, return the existing range
        if (it_from != date_to_balance_.end() && it_to != date_to_balance_.end()) {
            return Range(it_from,it_to);
        } else if (it_from == date_to_balance_.end() && it_to != date_to_balance_.end()) {
            Date insertion{to};
            while(insertion >= from) {
                date_to_balance_[--insertion];
            }
            return Range(date_to_balance_.find(from),it_to);
        } else if (it_to == date_to_balance_.end() && it_from != date_to_balance_.end()) {
            Date insertion{from};
            while(insertion <= from) {
                date_to_balance_[++insertion];
            }
            return Range(it_from,date_to_balance_.find(to));
        // Neither found
        } else {
            Date insertion{from};
            while(insertion <= to) {
                date_to_balance_[insertion++];
            }
            return Range(date_to_balance_.find(from),date_to_balance_.find(to));
        }
    }
};

void TestDateOperators() {
    BudgetManager::Date bigger{.year = 1990, .month = 6, .day = 21};
    BudgetManager::Date smaller{.year = 1990, .month = 6, .day = 20};
    ASSERT(smaller < bigger);
    ASSERT(bigger > smaller);
    ASSERT(bigger >= smaller);
    ASSERT(smaller <= bigger);
    ASSERT(smaller != bigger);
    bigger = {.year = 1990, .month = 6, .day = 20};
    ASSERT(smaller == bigger);
    ASSERT(smaller == bigger);
}

void TestDateIncDec() {
    BudgetManager::Date result{.year = 1990, .month = 12, .day = 30};
    BudgetManager::Date expected{.year = 1990, .month = 12, .day = 31};
    ASSERT(++result == expected);
    result++;
    expected = BudgetManager::Date{.year = 1991, .month = 1, .day = 1};
    ASSERT(result == expected);
    expected = BudgetManager::Date{.year = 1990, .month = 12, .day = 31};
    ASSERT(--result == expected);
    result--;
    expected = BudgetManager::Date{.year = 1990, .month = 12, .day = 30};
    ASSERT(result == expected);
    result++;
    ++result;
    expected = BudgetManager::Date{.year = 1991, .month = 1, .day = 1};
    ASSERT(result == expected);
    result--;
    --result;
    expected = BudgetManager::Date{.year = 1990, .month = 12, .day = 30};
    ASSERT(result == expected);
}

void TestGetDatesRange() {
    BudgetManager bm;
    BudgetManager::Date from{.year = 1991, .month = 1, .day = 1};
    BudgetManager::Date to{.year = 1991, .month = 2, .day = 1};
    auto range = bm.GetDatesRange(from,to);
    for(const auto& date : range) {
        ASSERT(date.first == from++);
    }
    from = {.year = 1991, .month = 1, .day = 1};
    range = bm.GetDatesRange(from,to);
    for(const auto& date : range) {
        ASSERT(date.first == from++);
    }
}

int main(void) {
    TestRunner tr;
    RUN_TEST(tr,TestDateOperators);
    RUN_TEST(tr,TestDateIncDec);
    RUN_TEST(tr,TestGetDatesRange);
    return 0;
}