#pragma once
#include <vector>
#include <string>

namespace YP {
    struct WorkingTimeInterval {
        enum class Day {
            EVERYDAY,
            MONDAY,
            TUESDAY,
            WEDNESDAY,
            THURSDAY,
            FRIDAY,
            SATURDAY,
            SUNDAY
        };

        Day day;
        int minutes_from;
        int minutes_to;
    };

    struct WorkingTime {
        WorkingTime() = default;
        template<typename T,ENABLE_IF_TYPE_IS(T,std::vector<WorkingTimeInterval>)>
        void SetIntervals(T&& intervals_) {
            intervals = std::forward<T>(intervals_);
        }
        void AddInterval(WorkingTimeInterval::Day day,int from,int to) {
            intervals.push_back({day,from,to});
        }
        std::string formatted;
        std::vector<WorkingTimeInterval> intervals;
    };
}