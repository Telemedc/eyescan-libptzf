#include <iostream>
#include <experimental/optional>
#include <string>
#include <variant>

#include <nlohmann/json.hpp>

#include "position.h"
#include "utils.h"
#include "ptzf_config.h"

using std::experimental::optional;
using std::experimental::nullopt;

namespace ptzf {

Position
Position::from_json(const std::string& s) {
  auto parsed = nlohmann::json::parse(s);
  return Position{
    parsed["x"].get<double>(),
    parsed["y"].get<double>(),
    parsed["z"].get<double>(),
    parsed["f"].get<double>()};
}

std::string
Position::to_json() const {
  nlohmann::json j;
  j["x"] = x;
  j["y"] = y;
  j["z"] = z;
  j["f"] = f;
  return j.dump();
}

optional<Position>
Position::from_gcode(const std::string &line) {
  return string_to_position(line);
}

std::string
Position::to_gcode() const {
  return position_to_string(*this);
}

}  // namespace ptzf
