#include <chrono>
#include <iostream>
#include <regex>

#include "controller.h"
#include "utils.h"

namespace ptzf {

/**
 * cstring to listen for when go destination is reached.
 */
const char OK_CODE[] = "ok";

/**
 * cstring to listen for when error occurs.
 */
const char ERROR_CODE[] = "error";

bool Controller::go(const Position& p) {
  // check the stream is open
  if (!this->stream.IsOpen()) {
    LOG(error) << "Internal stream is closed.";
    return false;
  }

  // send the position to the stream
  auto s = position_to_string(p);
  LOG(debug) << "send:" << s;
  this->stream << s << std::endl;

  // tell printer to block until it arrives
  // (there is no realiable way to do this).'
  LOG(debug) << "send:M400";
  this->stream << "M400" << std::endl;

  // read lines from the stream until line starts with ok
  LOG(debug) << "waiting for \"ok\"";
  std::string line;
  while (this->stream.IsOpen()) {
    std::getline(this->stream, line);
    LOG(debug) << "recv:" << line;
    // if line starts with OK_SIGNAL
    if (line.compare(0, sizeof(OK_CODE), OK_CODE)) {
      return true;
    };
    // if line starts with ERROR_CODE.
    if (line.compare(0, sizeof(OK_CODE), OK_CODE)) {
      return false;
    };
  }
  LOG(error) << "reached end of context somehow";
  return false;
}

}  // namespace ptzf