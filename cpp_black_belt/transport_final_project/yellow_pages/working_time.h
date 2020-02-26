#pragma once
#include <vector>
#include <string>

namespace YellowPages {
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
        std::string formatted;
        std::vector<WorkingTimeInterval> intervals;
    };
}