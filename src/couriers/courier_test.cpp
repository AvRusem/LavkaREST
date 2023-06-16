#include <gtest/gtest.h>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/json/value.hpp>

#include "courier.hpp"

using namespace lavka;
using namespace lavka::couriers;

const userver::formats::json::Value kJson = userver::formats::json::FromString(R"({
    "couriers": [
    {
      "courier_type": "FOOT",
      "regions": [1, 2, 3],
      "working_hours": ["09:00-12:00", "14:00-18:00"]
    },
    {
      "courier_type": "BIKE",
      "regions": [4, 5],
      "working_hours": ["10:00-13:00", "15:00-19:00"]
    }
  ]
  })");

TEST(Courier, TypeParser) {
    CourierType type = kJson["couriers"][0]["courier_type"].As<CourierType>();
    EXPECT_EQ(CourierType::FOOT, type);

    type = kJson["couriers"][1]["courier_type"].As<CourierType>();
    EXPECT_EQ(CourierType::BIKE, type);
}

TEST(Courier, WorkingHoursParser) {
    std::vector<TimeRange> time_range = kJson["couriers"][0]["working_hours"].As<std::vector<lavka::TimeRange>>();
    TimePoint expected_time_start("09:00");
    TimePoint expected_time_end("12:00");
    EXPECT_EQ(expected_time_start, time_range[0].GetStart());
    EXPECT_EQ(expected_time_end, time_range[0].GetEnd());

    time_range = kJson["couriers"][1]["working_hours"].As<std::vector<lavka::TimeRange>>();
    expected_time_start = TimePoint("15:00");
    expected_time_end = TimePoint("19:00");
    EXPECT_EQ(expected_time_start, time_range[1].GetStart());
    EXPECT_EQ(expected_time_end, time_range[1].GetEnd());
}

TEST(Courier, CourierParser) {
    std::vector<Courier> couriers = kJson["couriers"].As<std::vector<Courier>>();
    EXPECT_EQ(CourierType::FOOT, couriers[0].type);
    EXPECT_EQ(CourierType::BIKE, couriers[1].type);

    EXPECT_EQ(std::vector<int>({1, 2, 3}), couriers[0].regions);
    EXPECT_EQ(std::vector<int>({4, 5}), couriers[1].regions);

    EXPECT_EQ(TimePoint("14:00"), couriers[0].working_hours[1].GetStart());
    EXPECT_EQ(TimePoint("18:00"), couriers[0].working_hours[1].GetEnd());
    EXPECT_EQ(TimePoint("10:00"), couriers[1].working_hours[0].GetStart());
    EXPECT_EQ(TimePoint("13:00"), couriers[1].working_hours[0].GetEnd());
}
