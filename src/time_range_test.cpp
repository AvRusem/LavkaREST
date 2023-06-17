#include <vector>

#include <gtest/gtest.h>
#include <userver/utest/utest.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/formats/json/value.hpp>

#include "time_range.hpp"

using namespace lavka;

class TimeRangeTest : public ::testing::Test {
protected:
    userver::formats::json::Value json_;

    void SetUp() override {
        json_ = userver::formats::json::FromString(R"(
            ["09:00-12:00", "14:00-18:00"]
        )");
    }

};

TEST_F(TimeRangeTest, Parser) {
    std::vector<TimeRange> time_range = json_.As<std::vector<lavka::TimeRange>>();

    TimePoint expected_time_start("09:00");
    TimePoint expected_time_end("12:00");
    EXPECT_EQ(expected_time_start, time_range[0].GetStart());
    EXPECT_EQ(expected_time_end, time_range[0].GetEnd());

    expected_time_start = TimePoint("14:00");
    expected_time_end = TimePoint("18:00");
    EXPECT_EQ(expected_time_start, time_range[1].GetStart());
    EXPECT_EQ(expected_time_end, time_range[1].GetEnd());
}

TEST_F(TimeRangeTest, Serialize) {
    TimeRange time_range("10:00", "20:00");

    userver::formats::json::ValueBuilder builder;
    builder["time_range"] = time_range;

    auto json = builder.ExtractValue();
    auto expected_json = userver::formats::json::FromString(R"({
            "time_range": "10:00-20:00"
        })");
    EXPECT_EQ(expected_json, json);

    EXPECT_EQ(time_range.GetStart(), json["time_range"].As<lavka::TimeRange>().GetStart());
    EXPECT_EQ(time_range.GetEnd(), json["time_range"].As<lavka::TimeRange>().GetEnd());

    builder["time_range"].PushBack(userver::formats::json::ValueBuilder(time_range));
    builder["time_range"].PushBack(userver::formats::json::ValueBuilder(time_range));

    json = builder.ExtractValue();
    expected_json = userver::formats::json::FromString(R"({
            "time_range": ["10:00-20:00", "10:00-20:00"]
        })");
    EXPECT_EQ(expected_json, json);

    EXPECT_EQ(time_range.GetStart(), json["time_range"][0].As<lavka::TimeRange>().GetStart());
    EXPECT_EQ(time_range.GetEnd(), json["time_range"][1].As<lavka::TimeRange>().GetEnd());
}
