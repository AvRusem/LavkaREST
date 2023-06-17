#include <vector>

#include <gtest/gtest.h>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/json/value.hpp>

#include "time_range.hpp"

using namespace lavka;

class TimeRangeTest : public ::testing::Test {
protected:
    userver::formats::json::Value json;

    void SetUp() override {
        json = userver::formats::json::FromString(R"(
            ["09:00-12:00", "14:00-18:00"]
        )");
    }

};

TEST_F(TimeRangeTest, Parser) {
    std::vector<TimeRange> time_range = json.As<std::vector<lavka::TimeRange>>();

    TimePoint expected_time_start("09:00");
    TimePoint expected_time_end("12:00");
    EXPECT_EQ(expected_time_start, time_range[0].GetStart());
    EXPECT_EQ(expected_time_end, time_range[0].GetEnd());

    expected_time_start = TimePoint("14:00");
    expected_time_end = TimePoint("18:00");
    EXPECT_EQ(expected_time_start, time_range[1].GetStart());
    EXPECT_EQ(expected_time_end, time_range[1].GetEnd());
}
