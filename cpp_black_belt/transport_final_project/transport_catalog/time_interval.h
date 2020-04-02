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
    Time(size_t day_, size_t hour_, size_t mins_)
    : day(static_cast<DayT>(day_)), mins(hour_ * 60 + mins_) {}
    Time(size_t day_, size_t mins_)
    : day(static_cast<DayT>(day_)), mins(mins_) {}

    size_t Mins() const { return mins; }
    DayT Day() const { return day; }
private:
    DayT day;
    size_t mins;
};

class TimeInterval {
public:
    TimeInterval(size_t day_, size_t mins_from_, size_t mins_to_)
    : t(day_, mins_from_), mins_to(mins_to_) 
    {}
    size_t MinsFrom() const { return t.Mins(); }
    size_t MinsTo() const { return mins_to; }
    Time::DayT Day() const { return t.Day(); }
private:
    Time t;
    size_t mins_to;
};

size_t GetWaitingTime(const Time& start_time, const std::vector<TimeInterval>& intervals);