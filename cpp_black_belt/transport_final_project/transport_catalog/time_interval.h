#pragma once

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

#include <algorithm>
static size_t GetWaitingTime(const Time& start_time, const std::vector<TimeInterval>& intervals) {
    auto f_process_day_intervals = 
        [] (auto it_begin, auto it_end, size_t mins) -> std::optional<size_t> {
            auto it_lower = std::lower_bound(it_begin, it_end, mins,
            [](const TimeInterval& interval, size_t time){
                return interval.MinsTo() < time;
            });
            auto it_upper = std::upper_bound(it_begin, it_end, mins,
            [](size_t time, const TimeInterval& interval){
                return interval.MinsTo() < time;
            });
            if(it_lower == it_upper) {
                return std::nullopt;
            } else if(it_upper->MinsFrom() > mins) {
                return it_upper->MinsFrom() - mins;
            }
            return 0ul;
        };

    auto f_find_day = 
        [&intervals] (Time::DayT day) 
        -> std::optional<std::pair<std::vector<TimeInterval>::const_iterator,std::vector<TimeInterval>::const_iterator>> 
        {
            auto it_lower = std::lower_bound(intervals.begin(), intervals.end(), day,
            [](const TimeInterval& interval, Time::DayT day_){
                return static_cast<size_t>(interval.Day()) < static_cast<size_t>(day_);
            });
            auto it_upper = std::upper_bound(intervals.begin(), intervals.end(), day,
            [](Time::DayT day_, const TimeInterval& interval){
                return static_cast<size_t>(interval.Day()) < static_cast<size_t>(day_);
            });
            
            if(it_lower == it_upper) {
                return std::nullopt;
            } else {
                return std::pair{it_lower,it_upper};
            }
        };

    if(intervals.empty()) {
        return 0;
    }
    if(intervals[0].Day() == Time::DayT::EVERYDAY) {
        auto res = f_process_day_intervals(intervals.begin(),intervals.end(),start_time.Mins());
        if(res) {
            return *res;
        } else {
            return *f_process_day_intervals(
                intervals.begin(),intervals.end(),0ul
            ) + 24 * 60 - start_time.Mins();
        }
    } else {
        size_t cur_mins = start_time.Mins();
        size_t acced_mins = 0;
        for(auto day = start_time.Day();;day = Time::IncDay(day)) {
            if(auto opt_it = f_find_day(day); opt_it) {
                auto [it_begin,it_end] = *opt_it;
                auto res = f_process_day_intervals(it_begin,it_end,cur_mins);
                if(res) {
                    return *res + acced_mins;
                }
            }
            acced_mins += 24 * 60 - cur_mins;
        }
    }
    return 666;
}