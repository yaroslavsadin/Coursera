#pragma once
#include <algorithm>

class Time {
public:
    enum class DayT {
        EVERYDAY, // for intervals
        MONDAY,
        TUESDAY,
        WEDNESDAY,
        THURSDAY,
        FRIDAY,
        SATURDAY,
        SUNDAY
    };

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
private:
    DayT day;
    double mins;
};

class TimeInterval {
public:
    TimeInterval(size_t day_, double mins_from_, double mins_to_)
    : t(day_, mins_from_), mins_to(mins_to_) 
    {}
    double MinsFrom() const { return t.Mins(); }
    double MinsTo() const { return mins_to; }
    Time::DayT Day() const { return t.Day(); }
private:
    Time t;
    double mins_to;
};

double GetWaitingTime(const Time& start_time, const std::vector<TimeInterval>& intervals);