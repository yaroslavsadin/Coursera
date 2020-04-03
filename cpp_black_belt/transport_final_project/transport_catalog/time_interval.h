#pragma once
#include <algorithm>
#include <vector>
#include <optional>

constexpr auto MINS_IN_DAY = 24 * 60ul;

enum class DayT {
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY
};

class Time {
public:
    static DayT IncDay(DayT day) {
        return (day == DayT::SUNDAY) ? 
                DayT::MONDAY : 
                static_cast<DayT>(static_cast<size_t>(day) + 1);
    }
    
    Time() = default;
    Time(size_t day_, double hour_, double mins_)
    : day(static_cast<DayT>(day_)), mins(hour_ * 60 + mins_) {}
    Time(size_t day_, double mins_)
    : day(static_cast<DayT>(day_)), mins(mins_) {}

    double Mins() const { return mins; }
    DayT Day() const { return day; }
    Time operator+(double mins) const {
        auto day_cnt = 0ul;
        while(mins > MINS_IN_DAY) {
            mins -= MINS_IN_DAY;
            day_cnt++;
        }
        auto new_mins = mins + this->mins;
        if(new_mins > MINS_IN_DAY) {
            new_mins -= MINS_IN_DAY;
            day_cnt++;
        }
        DayT new_day = this->day;
        while(day_cnt--) {
            new_day = IncDay(new_day);
        }
        return Time(static_cast<size_t>(new_day),new_mins);
    }
private:
    DayT day;
    double mins;
};

class TimeInterval {
public:
    TimeInterval(size_t day_, double mins_from_, double mins_to_)
    : day((day_ == 0) ? std::nullopt : std::optional<DayT>(static_cast<DayT>(day_ - 1ul))),
      mins_from(mins_from_), mins_to(mins_to_) 
    {}
    double MinsFrom() const { return mins_from; }
    double MinsTo() const { return mins_to; }
    auto Day() const { return day; }
private:
    std::optional<DayT> day;
    double mins_from;
    double mins_to;
};

double GetWaitingTime(const Time& start_time, const std::vector<TimeInterval>& intervals);