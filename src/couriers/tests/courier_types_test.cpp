#include "couriers/courier_types.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/utest/utest.hpp>

using namespace userver;

UTEST(CourierTypes, SuccessJsonParse) {
  using lavka::CourierType;

  formats::json::Value json = formats::json::FromString(R"({
  "foot": "FOOT",
  "bike": "BIKE",
  "auto": "AUTO"
  })");

  EXPECT_EQ(json["foot"].As<CourierType>(), CourierType::kFoot);
  EXPECT_EQ(json["bike"].As<CourierType>(), CourierType::kBike);
  EXPECT_EQ(json["auto"].As<CourierType>(), CourierType::kAuto);
}

UTEST(CourierTypes, ExceptionJsonParse) {
  using lavka::CourierType;

  formats::json::Value json = formats::json::FromString(R"({
  "invalid": "INVALID"
  })");

  EXPECT_THROW(json["invalid"].As<CourierType>(), formats::json::Exception);
}

UTEST(CourierTypes, SuccessSerialize) {
  using lavka::CourierType;

  CourierType foot = CourierType::kFoot;
  CourierType bike = CourierType::kBike;
  CourierType auto_v = CourierType::kAuto;

  formats::json::ValueBuilder builderJson;

  builderJson["foot"] = foot;
  builderJson["bike"] = bike;
  builderJson["auto"] = auto_v;
  auto json = builderJson.ExtractValue();
  EXPECT_EQ(json["foot"].As<std::string>(), "FOOT");
  EXPECT_EQ(json["bike"].As<std::string>(), "BIKE");
  EXPECT_EQ(json["auto"].As<std::string>(), "AUTO");
}
