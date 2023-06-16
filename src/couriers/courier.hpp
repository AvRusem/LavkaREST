#pragma once

#include <vector>

#include <userver/utils/time_of_day.hpp>
#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/to.hpp>

#include "../time_range.hpp"

namespace lavka 
{

namespace couriers
{

enum CourierType {
    FOOT,
    BIKE,
    AUTO
};

CourierType Parse(const userver::formats::json::Value& json,
                 userver::formats::parse::To<CourierType>);

struct Courier {
    int64_t id;
    CourierType type;
    std::vector<int32_t> regions;
    std::vector<lavka::TimeRange> working_hours;
};

Courier Parse(const userver::formats::json::Value& json,
                 userver::formats::parse::To<Courier>);

} // namespace couriers

} // namespace lavka
