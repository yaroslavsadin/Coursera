#include <iostream>
#include "test_runner.h"
#include "time_interval.h"

void TestTimeIntervals() {
    {    
        std::vector<TimeInterval> intervals;
        intervals.emplace_back(1,100,200);
        intervals.emplace_back(2,300,400);
        intervals.emplace_back(3,100,200);
        intervals.emplace_back(3,500,700);
        intervals.emplace_back(3,1000,1100);
        intervals.emplace_back(4,100,200);
        intervals.emplace_back(4,400,800);
        intervals.emplace_back(5,100,200);
        intervals.emplace_back(6,100,200);
        intervals.emplace_back(7,100,200);
        {
            Time current_time(static_cast<size_t>(Time::DayT::MONDAY),0);
            ASSERT_EQUAL(GetWaitingTime(current_time,intervals),100ul);
        }
        {
            Time current_time(static_cast<size_t>(Time::DayT::MONDAY),100);
            ASSERT_EQUAL(GetWaitingTime(current_time,intervals),0ul);
        }
        {
            Time current_time(static_cast<size_t>(Time::DayT::MONDAY),101);
            ASSERT_EQUAL(GetWaitingTime(current_time,intervals),0ul);
        }
        {
            Time current_time(static_cast<size_t>(Time::DayT::MONDAY),200);
            ASSERT_EQUAL(GetWaitingTime(current_time,intervals),1540ul);
        }
        {
            Time current_time(static_cast<size_t>(Time::DayT::WEDNESDAY),200);
            ASSERT_EQUAL(GetWaitingTime(current_time,intervals),300ul);
        }
        {
            Time current_time(static_cast<size_t>(Time::DayT::WEDNESDAY),700);
            ASSERT_EQUAL(GetWaitingTime(current_time,intervals),300ul);
        }
        {
            Time current_time(static_cast<size_t>(Time::DayT::THURSDAY),200);
            ASSERT_EQUAL(GetWaitingTime(current_time,intervals),200ul);
        }
        {
            Time current_time(static_cast<size_t>(Time::DayT::THURSDAY),300);
            ASSERT_EQUAL(GetWaitingTime(current_time,intervals),100ul);
        }
        {
            Time current_time(static_cast<size_t>(Time::DayT::SUNDAY),200);
            ASSERT_EQUAL(GetWaitingTime(current_time,intervals),1340);
        }
    }
    {
        std::vector<TimeInterval> intervals;
        intervals.emplace_back(0,100,200);
        intervals.emplace_back(0,300,400);
        intervals.emplace_back(0,800,1000);

        {
            Time current_time(static_cast<size_t>(Time::DayT::MONDAY),0);
            ASSERT_EQUAL(GetWaitingTime(current_time,intervals),100ul);
        }
        {
            Time current_time(static_cast<size_t>(Time::DayT::SATURDAY),300);
            ASSERT_EQUAL(GetWaitingTime(current_time,intervals),0ul);
        }
        {
            Time current_time(static_cast<size_t>(Time::DayT::FRIDAY),700);
            ASSERT_EQUAL(GetWaitingTime(current_time,intervals),100ul);
        }
        {
            Time current_time(static_cast<size_t>(Time::DayT::TUESDAY),1000);
            ASSERT_EQUAL(GetWaitingTime(current_time,intervals),540ul);
        }
        {
            Time current_time(static_cast<size_t>(Time::DayT::WEDNESDAY),1000);
            ASSERT_EQUAL(GetWaitingTime(current_time,intervals),540ul);
        }
        {
            Time current_time(static_cast<size_t>(Time::DayT::THURSDAY),234);
            ASSERT_EQUAL(GetWaitingTime(current_time,intervals),66ul);
        }
    }
}

int main(void) {
    TestRunner tr;
    RUN_TEST(tr,TestTimeIntervals);
    return 0;
}