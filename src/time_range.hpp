#pragma once

#include <string>

#include <userver/utils/time_of_day.hpp>
#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/to.hpp>
#include <userver/formats/serialize/to.hpp>

namespace lavka
{
    
typedef userver::utils::datetime::TimeOfDay<std::chrono::minutes> TimePoint;

class TimeRange {
private:
    TimePoint start_;
    TimePoint end_;

public:
    TimeRange() = default;
    TimeRange(std::string start, std::string end);

    TimePoint GetStart() const;
    TimePoint GetEnd() const;

};

TimeRange Parse(const userver::formats::json::Value& json,
                 userver::formats::parse::To<TimeRange>);

userver::formats::json::Value Serialize(const TimeRange& data,
                               userver::formats::serialize::To<userver::formats::json::Value>);

} // namespace lavka
