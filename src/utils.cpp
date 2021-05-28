/* Copyright 2020 Dandem Labs
 * Authored by Michael de Gans <mike@mdegans.dev>
 *
 * This file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <iomanip>
#include <regex>
#include <string>

#include <boost/log/trivial.hpp>
#define LOG BOOST_LOG_TRIVIAL

#include "utils.h"

using std::experimental::optional;

namespace ptzf {

/**
 * Regex for a position line (M114 return)
 */
const std::regex RX_LINE(
    R"ESC(X:(\d{1,2}\.\d{2}) Y:(\d{1,2}\.\d{2}) Z:(\d{1,2}\.\d{2}) E:(\d{1,2}\.\d{2}) Count X)ESC");

optional<Position> string_to_position(const std::string& line) {
  LOG(debug) << "Parsing:" << line;
  std::smatch match;
  if (!std::regex_match(line, match, RX_LINE)) {
    // some random line, possibly an "ok ..."
    // todo: forward any errors to boost log
    return {};
  }
  try {
    // parse line here and return a Position if possible
    return Position{std::stof(match[0]), std::stof(match[1]),
                    std::stof(match[2]), std::stof(match[3])};
  } catch (const std::exception& e) {
    // this could happen if the matched values somehow could not convert to
    // float (this should never happen unless the regex is broken).
    LOG(error) << "Could not parse:" << line << " because " << e.what();
  }
  return {};
}

std::string position_to_string(const Position& p) {
  std::stringstream s;
  s << std::fixed << std::setprecision(3) << "G0 X" << p.x << " Y" << p.y
    << " Z" << p.z << " E" << p.f;
  return s.str();
}

}  // namespace ptzf
