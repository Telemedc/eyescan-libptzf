#include <experimental/optional>
#include <chrono>
#include <string>
#include <thread>
#include <limits>

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>

#include "utils.h"
#include "serial.h"
#include "position.h"

namespace logging = boost::log;

#define LOG BOOST_LOG_TRIVIAL

namespace {

constexpr auto _TRY_LIMIT_MAX = "G0 X999 Y999 Z999 E999";
constexpr auto _TRY_LIMIT_MIN = "G0 X-999 Y-999 Z-999 E-999";

} // namespace

namespace ptzf {

using std::experimental::optional;
using std::experimental::nullopt;

bool wait_for_ok(LibSerial::SerialStream& stream) {
#ifdef PRINTER
  if (!stream.IsOpen()) {
    LOG(error) << "Stream not open. Can't wait for \"OK\"";
    return false;
  }

  // read lines from the stream until line starts with ok
  LOG(debug) << "Waiting for \"OK\"";
  std::string line;
  int lines_parsed = 0;
  while (stream.IsOpen()) {
    std::getline(stream, line);
    LOG(debug) << "recv:" << line;
    // to prevent infinite loop in case of junk data from printer
    if (lines_parsed++ > 1000) {
      LOG(error) << "Could not find OK or ERROR. `wait_for_ok` failed.";
      return false;
    }
    // if line starts with OK_CODE_P62
    if (!line.compare(0, sizeof(OK_CODE_P62), OK_CODE_P62)) {
      LOG(debug) << "Found OK_CODE_P62 in line: \"" << line << '"';
      return true;
    };
    // if line starts with OK_CODE_P62
    if (!line.compare(0, sizeof(OK_CODE_P63), OK_CODE_P63)) {
      LOG(debug) << "Found OK_CODE_P63 in line: \"" << line << '"';
      return true;
    };
    // if line starts with BUSY_CODE
    if (!line.compare(0, sizeof(BUSY_CODE), BUSY_CODE)) {
      LOG(debug) << "Found BUSY_CODE in line: \"" << line << '"';
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
      continue;
    };
    // if line starts with ERROR_CODE.
    if (!line.compare(0, sizeof(ERROR_CODE), ERROR_CODE)) {
      LOG(error) << line;
      return false;
    };
  }

  LOG(error) << "reached end of context somehow";
  return false;
#else
  (void)stream;
  LOG(debug) << "no PRINTER. wait_for_ok returning true";
  return true;
#endif
}

// NOTE: position is not const correct because it's members are not all const
// Credit to @lackdaz for the logic here. Ported from Python.
static const optional<Position>
get_limit(LibSerial::SerialStream &stream, const char* gcode_go) {
  if (!stream.IsOpen()) {
    LOG(error) << "Can't get limits from a stream when not IsOpen()";
    return nullopt;
  }
  // request to move past limits
  LOG(debug) << "Moving past limits to: " << gcode_go;
  stream << gcode_go << std::endl;  // NOTE: std::endl includes a flush
  // wait for ok (or error, we don't care)
  (void)wait_for_ok(stream);
  // flush input buffer (read until empty)
  // LOG(debug) << "flushing input buffer";
  // stream.ignore(std::numeric_limits<std::streamsize>::max());
  // request current position
  LOG(debug) << "Requesting current position (M114)";
  stream << "M114" << std::endl;  // NOTE: std::endl includes a flush
  // get the response. the printer returns a line like this:
  // "recv:X:0.00 Y:0.00 Z:0.00 E:0.00 Count X:0 Y:0 Z:0 E:0"
  // and then an ok.
  for (int i = 0; i < 100; i++) {
    std::string line;
    std::getline(stream, line);
    LOG(debug) << "m114 resp:" << line;
    auto pos = string_to_position(line);
    if (pos) {
      if (!wait_for_ok(stream)) {
        LOG(warning) << "Got position but ok failed somehow: " << pos.value().to_gcode();
      }
      return pos;
    }
  }

  // failed 
  return nullopt;
}

const optional<Position>
get_minimum_limit(LibSerial::SerialStream &stream) {
#ifdef PRINTER
  return get_limit(stream, _TRY_LIMIT_MIN);
#else
  (void)_TRY_LIMIT_MIN,
  (void)get_limit;
  (void)stream;
  return Position{0,0,0,0};
#endif
}

const optional<Position>
get_maximum_limit(LibSerial::SerialStream &stream) {
#ifdef PRINTER
  return get_limit(stream, _TRY_LIMIT_MAX);
#else
  (void)_TRY_LIMIT_MAX,
  (void)get_limit;
  (void)stream;
  return Position{999, 999, 999, 999};
#endif
}


}