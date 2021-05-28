#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>

#include <SerialStream.h>

#include "controller.h"
#include "utils.h"

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>

namespace logging = boost::log;

#define LOG BOOST_LOG_TRIVIAL

namespace ptzf {

/**
 * cstring to listen for when go destination is reached.
 */
const char OK_CODE[] = "ok P63 B31";

/**
 * code to indicate printer is busy
 */
const char BUSY_CODE[] = "echo:busy:";

/**
 * cstring to listen for when error occurs.
 */
const char ERROR_CODE[] = "error";

static bool wait_for_ok(LibSerial::SerialStream& stream) {
  if (!stream.IsOpen()) {
    LOG(error) << "Stream not open. Can't wait for \"OK\"";
    return false;
  }

  LOG(debug) << "Waiting for \"OK\"";
  std::string line;
  while (stream.IsOpen()) {
    std::getline(stream, line);
    LOG(debug) << "recv:" << line;
    // if line starts with OK_CODE
    if (!line.compare(0, sizeof(OK_CODE), OK_CODE)) {
      LOG(debug) << "Found OK_CODE in line: \"" << line << '"';
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
}

class Controller::Impl {
 public:
  Impl(std::string device, bool do_connect = true) : device(device), stream() {
    // this isn't ideal since the logger is global.
    // TODO: logger just for ptzf
    if (std::getenv("PTZF_DEBUG")) {
      LOG(info) << "ptzf debug logging enabled";
    } else {
      // only errors and stuff
      logging::core::get()->set_filter(logging::trivial::severity >=
                                       logging::trivial::info);
    }
    if (do_connect) {
      this->connect();
    }
  }
  ~Impl() = default;

  bool connect() {
    try {
      LOG(debug) << "Opening: " << this->device;
      this->stream.Open(this->device, std::ios::in | std::ios::out);
    } catch (const SerialPort::OpenFailed& e) {
      LOG(error) << "Open failed because: " << e.what();
      return false;
    }
    if (!this->is_connected()) {
      // this can happen becuase libserial does not always throw OpenFailed on
      // failure to open.q
      LOG(error) << "Open failed for unknown reason.";
      return false;
    }
    LOG(debug) << "Opened: " << this->device;
    this->stream.SetBaudRate(LibSerial::SerialStreamBuf::BAUD_115200);
    LOG(debug) << "Baud set: 115200";
    // note: std::endl includes a flush
    this->stream << "G0 F2400" << std::endl;
    LOG(debug) << "Speed set: 2400mm/min";
    this->stream << "G28" << std::endl;
    LOG(debug) << "G28 Auto Home set";

    return wait_for_ok(this->stream);
  }

  bool disconnect() {
    // this signals all threads to stop
    if (this->stream.IsOpen()) {
      LOG(debug) << "Closing: " << this->device;
      this->stream.Close();
    }
    LOG(debug) << "Closed: " << this->device;
    return true;
  }

  bool is_connected() { return this->stream.IsOpen(); }

  bool go(Position p) {
    // check the stream is open
    if (!this->stream.IsOpen()) {
      LOG(error) << "Internal stream is closed.";
      return false;
    }

    // reset input buffer
    this->stream.sync();

    // send the position to the stream
    auto s = position_to_string(p);
    LOG(debug) << "send:" << s;
    this->stream << s << std::endl;

    // tell printer to block until it arrives
    // (there is no realiable way to do this).'
    LOG(debug) << "send:M400";
    this->stream << "M400" << std::endl;

    // read lines from the stream until line starts with ok
    return wait_for_ok(this->stream);
  }

 private:
  /** The device name */
  std::string device;
  /** LibSerial::SerialStream for the device */
  LibSerial::SerialStream stream;
};

Controller::Controller(std::string device, bool do_connect)
    : impl(std::make_unique<Impl>(device, do_connect)) {
}
Controller::~Controller() = default;


bool Controller::connect() {
  return impl->connect();
}

bool Controller::disconnect() {
  return impl->disconnect();
}

bool Controller::is_connected() const {
  return impl->is_connected();
}

bool Controller::go(Position p) {
  return impl->go(p);
}

}  // namespace ptzf