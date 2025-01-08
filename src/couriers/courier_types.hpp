#ifndef LAVKA_COURIERS_COURIER_TYPES_HPP
#define LAVKA_COURIERS_COURIER_TYPES_HPP

#include <string_view>

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/to.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/formats/serialize/to.hpp>
#include <userver/storages/postgres/io/enum_types.hpp>
#include <userver/storages/postgres/io/io_fwd.hpp>
#include <userver/utils/trivial_map.hpp>

namespace lavka {

enum class CourierType { kFoot, kBike, kAuto };

static constexpr userver::utils::TrivialBiMap kCourierTypesMap(
    [](auto selector) {
      return selector()
          .Case(CourierType::kFoot, "FOOT")
          .Case(CourierType::kBike, "BIKE")
          .Case(CourierType::kAuto, "AUTO");
    });

template <class Value>
CourierType Parse(const Value& data, userver::formats::parse::To<CourierType>) {
  return userver::utils::ParseFromValueString(data, kCourierTypesMap);
}

template <class Value>
Value Serialize(const CourierType& data,
                userver::formats::serialize::To<Value>) {
  return typename Value::Builder(kCourierTypesMap.TryFind(data)).ExtractValue();
}

}  // namespace lavka

namespace userver::storages::postgres::io {

template <>
struct CppToUserPg<lavka::CourierType> {
  static constexpr DBTypeName postgres_name = "couriers.delivery";
  static constexpr userver::utils::TrivialBiMap enumerators =
      lavka::kCourierTypesMap;
};

}  // namespace userver::storages::postgres::io

#endif  // LAVKA_COURIERS_COURIER_TYPES_HPP
