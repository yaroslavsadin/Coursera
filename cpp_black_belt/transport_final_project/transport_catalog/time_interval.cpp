#include "time_interval.h"
#include <cassert>

template<typename It> 
std::optional<std::pair<It,It>> FindDayIntervals(Time::DayT day, It intervals_begin, It intervals_end)  
{
    auto it_lower = std::lower_bound(intervals_begin, intervals_end, day,
    [](const TimeInterval& interval, Time::DayT day_){
        return static_cast<size_t>(interval.Day()) < static_cast<size_t>(day_);
    });
    auto it_upper = std::upper_bound(intervals_begin, intervals_end, day,
    [](Time::DayT day_, const TimeInterval& interval){
        return static_cast<size_t>(day_) < static_cast<size_t>(interval.Day());
    });
    
    for(auto it = it_lower; it != it_upper; it++) {
        assert(it->Day() == day);
    }

    if(it_lower == it_upper) {
        return std::nullopt;
    } else {
        return std::pair{it_lower,it_upper};
    }
};

template<typename It>
std::optional<size_t> ProcessDayIntervals (It it_begin, It it_end, size_t mins) {
    auto it = std::upper_bound(it_begin, it_end, mins,
    [](size_t time, const TimeInterval& interval){
        return time < interval.MinsTo();
    });
    if(it == it_end) {
        return std::nullopt;
    } else if(it->MinsFrom() > mins) {
        return it->MinsFrom() - mins;
    }
    return 0ul;
};

size_t GetWaitingTime(const Time& start_time, const std::vector<TimeInterval>& intervals) {
    if(intervals.empty()) {
        return 0;
    }
    if(intervals[0].Day() == Time::DayT::EVERYDAY) {
        auto res = ProcessDayIntervals(intervals.begin(),intervals.end(),start_time.Mins());
        if(res) {
            return *res;
        } else {
            // Wait till the next day
            return *ProcessDayIntervals(intervals.begin(),intervals.end(),0ul) + 24 * 60 - start_time.Mins();
        }
    } else {
        size_t cur_mins = start_time.Mins();
        size_t acced_mins = 0;
        for(auto day = start_time.Day();;day = Time::IncDay(day)) {
            if(auto opt_it = FindDayIntervals(day,intervals.begin(),intervals.end()); opt_it) {
                auto [it_begin,it_end] = *opt_it;
                auto res = ProcessDayIntervals(it_begin,it_end,cur_mins);
                if(res) {
                    return *res + acced_mins;
                }
            }
            acced_mins += 24 * 60 - cur_mins;
            cur_mins = 0;
        }
    }
    return 666;
}