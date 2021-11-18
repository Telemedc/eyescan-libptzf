#include <chrono>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>

#include <SerialStream.h>

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>

#include "controller.h"
#include "utils.h"

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

#ifndef PRINTER
/**
 * @brief Mock printer travel speed. Tuned to match the real thing (2400/60).
 */
const double MOCK_MM_PER_SEC = 40.0;
/** @returns xyz distance between two ptzf Positions */
double distance_between(const ptzf::Position& a, const ptzf::Position& b) {
  double xd = (a.x - b.x) * (a.x - b.x);
  double yd = (a.y - b.y) * (a.y - b.y);
  double zd = (a.z - b.z) * (a.z - b.z);
  return std::sqrt(xd + yd + zd);
}
/** @returns simulated travel time between two ptzf Positions */
int ms_to_travel(const ptzf::Position& a, const ptzf::Position& b) {
  double xyz_d = distance_between(a, b);
  LOG(debug) << "simulated travel distance " << xyz_d << "mm";
  // focus can happen at the same time and is independent from xyz travel
  double fd = std::abs(a.f - b.f);
  LOG(debug) << "simulated focus distance " << fd << "mm";
  int ms = static_cast<int>(std::max(xyz_d, fd) / MOCK_MM_PER_SEC * 1000.0);
  LOG(debug) << "simulated travel time " << ms << "ms";
  return ms;
}
#endif  // PRINTER

static bool wait_for_ok(LibSerial::SerialStream& stream) {
  if (!stream.IsOpen()) {
    LOG(error) << "Stream not open. Can't wait for \"OK\"";
    return false;
  }

  // read lines from the stream until line starts with ok
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

struct Controller::Impl {
 public:
  Impl(std::string device, bool do_connect = true)
      : device(device)
#ifdef PRINTER
        ,
        stream()
#endif  // PRINTER
  {
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
#ifdef PRINTER
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
#else
    // it normally takes about 1000ms to connect
    std::this_thread::sleep_for(std::chrono::seconds(1));
    mock_connected = true;
    (void)wait_for_ok;  // because unused-function otherwise
    return mock_connected;
#endif  // PRINTER
  }

  bool disconnect() {
#ifdef PRINTER
    // this signals all threads to stop
    if (this->stream.IsOpen()) {
#endif  // PRINTER
      LOG(debug) << "Closing: " << this->device;
#ifdef PRINTER
      this->stream.Close();
    }
#else
    mock_connected = false;
#endif  // PRINTER
    LOG(debug) << "Closed: " << this->device;
    return true;
  }

  bool is_connected() {
#ifdef PRINTER
    return this->stream.IsOpen();
#else
    return mock_connected;
#endif  // PRINTER
  }

  bool go(Position p) {
    if (!p.is_valid()) {
      LOG(error) << "invalid position: " << position_to_string(p);
      return false;
    }
#ifdef PRINTER
    // check the stream is open
    if (!this->is_connected()) {
#else
    if (!mock_connected) {
#endif  // PRINTER
      LOG(error) << "Internal stream is closed.";
      return false;
    }
#ifdef PRINTER
    // reset input buffer
    this->stream.sync();
#endif  // PRINTER

    // send the position to the stream
    auto s = position_to_string(p);
    LOG(debug) << "send:" << s;
#ifdef PRINTER
    this->stream << s << std::endl;
#endif  // PRINTER

    // tell printer to block until it arrives
    // (there is no realiable way to do this).'
    LOG(debug) << "send:M400";
#ifdef PRINTER
    this->stream << "M400" << std::endl;
#endif  // PRINTER

#ifdef PRINTER
    return wait_for_ok(this->stream);
#else
    std::this_thread::sleep_for(
        std::chrono::milliseconds(ms_to_travel(p, mock_last_p)));
    this->mock_last_p = p;
    return true;
#endif  // PRINTER
  }

 private:
  /** The device name */
  std::string device;
#ifdef PRINTER
  /** LibSerial::SerialStream for the device */
  LibSerial::SerialStream stream;
#else
  bool mock_connected;
  ptzf::Position mock_last_p = {};
#endif  // PRINTER
};

Controller::Controller(std::string device, bool do_connect)
    : impl(std::make_unique<Impl>(device, do_connect)) {}
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