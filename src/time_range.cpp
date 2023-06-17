#include <regex>

#include <fmt/format.h>
#include <userver/formats/json/value_builder.hpp>

#include "time_range.hpp"

using namespace lavka;

TimeRange::TimeRange(std::string start, std::string end) :
            start_(start), end_(end) {}

TimePoint TimeRange::GetStart() const {
    return start_;
}

TimePoint TimeRange::GetEnd() const {
    return end_;
}

TimeRange lavka::Parse(const userver::formats::json::Value& json,
                 userver::formats::parse::To<TimeRange>) {
    std::regex pattern(R"(^(\d{2}:\d{2})-(\d{2}:\d{2})$)");

    std::string json_str = json.As<std::string>();
    std::smatch match;
    if (std::regex_search(json_str, match, pattern)) {
        std::string start_time = match.str(1);
        std::string end_time = match.str(2);

        return TimeRange(start_time, end_time);
    } else {
    }
}

userver::formats::json::Value lavka::Serialize(const TimeRange& data,
                               userver::formats::serialize::To<userver::formats::json::Value>) {
    std::string result_str(fmt::format("{}", data.GetStart()) + "-" + fmt::format("{}", data.GetEnd()));

    return userver::formats::json::Value::Builder(result_str).ExtractValue();
}
