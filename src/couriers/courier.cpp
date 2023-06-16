#include <string>

#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/parse/time_of_day.hpp>

#include "courier.hpp"

using namespace lavka::couriers;

Courier lavka::couriers::Parse(const userver::formats::json::Value& json,
                 userver::formats::parse::To<Courier>) {
    Courier courier;
    courier.type = json["courier_type"].As<CourierType>();
    courier.regions = json["regions"].As<std::vector<int32_t>>();
    courier.working_hours = json["working_hours"].As<std::vector<lavka::TimeRange>>();

    return courier;
}

CourierType lavka::couriers::Parse(const userver::formats::json::Value& json,
                                   userver::formats::parse::To<CourierType>) {
    std::string type_str = json.As<std::string>();

    if (type_str == "FOOT") {
        return CourierType::FOOT;
    }
    else if (type_str == "BIKE") {
        return CourierType::BIKE;
    }
    else if (type_str == "AUTO") {
        return CourierType::AUTO;
    }
}
