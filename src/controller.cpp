#include <chrono>
#include <cmath>
#include <experimental/optional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>

// on ubuntu 20.04 prefix "libserial/""
#ifdef LIBSERIAL_GE_1_0_0
#include <libserial/SerialStream.h>
#else
#include <SerialStream.h>
#endif

#include <nlohmann/json.hpp>

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>

#include "controller.h"
#include "position.h"
#include "serial.h"
#include "utils.h"

using std::experimental::nullopt;
using std::experimental::optional;

namespace logging = boost::log;

#define LOG BOOST_LOG_TRIVIAL

namespace ptzf {

Controller::Config Controller::Config::from_json(const std::string& s) {
  auto conf = nlohmann::json::parse(s);
  return Config{conf["device"].get<std::string>(),
                Position::from_json(conf["min"].get<std::string>()),
                Position::from_json(conf["max"].get<std::string>()),
                conf["do_connect"].get<bool>()};
}

std::string Controller::Config::to_json() const {
  nlohmann::json j;
  j["device"] = device;
  j["min"] = min.to_json();
  j["max"] = max.to_json();
  j["do_connect"] = do_connect;
  return j.dump();
}

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

struct Controller::Impl {
 public:
  Impl(Controller::Config&& config)
      : config(config),
        max_(nullopt),
        min_(nullopt)
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
    if (config.do_connect) {
      this->connect();
    }
  }
  ~Impl() = default;

  bool connect() {
#ifdef PRINTER
    try {
      LOG(debug) << "Opening: " << config.device;
      this->stream.Open(config.device, std::ios::in | std::ios::out);
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
    LOG(debug) << "Opened: " << config.device;
    this->stream.SetBaudRate(LibSerial::SerialStreamBuf::BAUD_115200);
    LOG(debug) << "Baud set: 115200";
    // note: std::endl includes a flush
    this->stream << "G0 F2400" << std::endl;
    if (!wait_for_ok(this->stream)) {
      LOG(error) << "Didn't get OK after `G0 F2400`";
      return false;
    }
    LOG(debug) << "Speed set: 2400mm/min";
    if (!go_home()) {
      LOG(error) << "Cannot connect becuase auto-home failed.";
      return false;
    }
    LOG(debug) << "G28 OK";
    LOG(debug) << "Getting limits from printer.";
    this->min_ = get_minimum_limit(this->stream);
    if (this->min_) {
      if (this->config.min < this->min_.value()) {
        LOG(warning) << "Config minimum (" << config.min.to_gcode()
                   << ") less than printer minimum ("
                   << this->min().value().to_gcode() << ")";
      }
      LOG(debug) << "Minimum limit" << this->min().value().to_gcode();
    }
    this->max_ = get_maximum_limit(this->stream);
    if (this->max_) {
      if (this->config.max > this->max_.value()) {
        LOG(warning) << "Config maximum (" << config.max.to_gcode()
                   << ") greater than printer maximum ("
                   << this->max().value().to_gcode() << ")";
      }
      LOG(debug) << "Actual Maximum limit:" << this->max().value().to_gcode();
    }
    if (!(this->max_ && this->min_)) {
      LOG(error) << "Failed to get limits from " PRINTER;
      return false;
    }
    return true;
#else
    // it normally takes about 1000ms to connect
    std::this_thread::sleep_for(std::chrono::seconds(1));
    mock_connected = true;
    this->min_ = Position{0, 0, 0, 0};
    this->max_ = Position{999, 999, 999, 999};
    return mock_connected;
#endif  // PRINTER
  }

  bool disconnect() {
#ifdef PRINTER
    // this signals all threads to stop
    if (this->stream.IsOpen()) {
#endif  // PRINTER
      LOG(debug) << "Closing: " << config.device;
#ifdef PRINTER
      this->stream.Close();
    }
#else
    mock_connected = false;
#endif  // PRINTER
    LOG(debug) << "Closed: " << config.device;
    return true;
  }

  bool is_connected() const {
#ifdef PRINTER
    return this->stream.IsOpen();
#else
    return mock_connected;
#endif  // PRINTER
  }

  bool go(Position p) {
    if (!is_valid_position(p)) {
      LOG(error) << "invalid position: " << p.to_gcode();
      return false;
    }
    // check the stream is open
    if (!this->is_connected()) {
      LOG(error) << "Internal stream is closed.";
      return false;
    }
#ifdef PRINTER
    // reset input buffer
    this->stream.sync();
#endif  // PRINTER

    // send the position to the stream
    auto s = p.to_gcode();
    LOG(debug) << "send:" << p.to_gcode();
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

  bool go_home() {
    LOG(debug) << "doing auto-home";
    // check the stream is open
    if (!this->is_connected()) {
      LOG(error) << "Internal stream is closed.";
      return false;
    }
#ifdef PRINTER
    this->stream << "G28" << std::endl;
    return wait_for_ok(this->stream);
#else
    mock_last_p = config.min;
    (void)wait_for_ok;
    return true;
#endif
  }

  optional<Position> max() {
    if (!is_connected()) {
      LOG(error) << "call `connect` first";
      return nullopt;
    }
    return std::min(max_.value(), config.max);
  }

  optional<Position> min() {
    if (!is_connected()) {
      LOG(error) << "can't get min position without connecting";
      return nullopt;
    }
    return std::max(min_.value(), config.min);
  }

  bool is_valid_position(Position p) {
    if (!is_connected()) {
      LOG(error) << "can't check valid position without connecting";
    }
    // check if within soft (Config) limits
    if (!config.is_valid_position(p)) {
      return false;
    }
    if (!(min_ && max_)) {
      LOG(error)
          << "no printer limits. likely `get_limits` or `connect` broken";
      return false;
    }
    return p <= max_.value() && p >= min_.value();
  }

  const Controller::Config config;

 private:
  optional<ptzf::Position> max_;
  optional<ptzf::Position> min_;
#ifdef PRINTER
  /** LibSerial::SerialStream for the device */
  LibSerial::SerialStream stream;
#else
  bool mock_connected;
  ptzf::Position mock_last_p = {};
#endif  // PRINTER
};

Controller::Controller(Config&& config)
    : impl(std::make_unique<Impl>(std::forward<Config>(config))) {}
Controller::Controller(std::string device, bool do_connect)
    : impl(std::make_unique<Impl>(
          Config(std::forward<std::string>(device), do_connect))) {}
Controller::~Controller() = default;

bool Controller::connect() {
  return impl->connect();
}

bool Controller::disconnect() {
  return impl->disconnect();
}

const Controller::Config& Controller::get_config() const {
  return impl->config;
}

bool Controller::is_connected() const {
  return impl->is_connected();
}

bool Controller::go(Position p) {
  return impl->go(p);
}

bool Controller::go_home() {
  return impl->go_home();
}

optional<Position> Controller::max() {
  return impl->max();
}

optional<Position> Controller::min() {
  return impl->min();
}

bool Controller::is_valid_position(Position p) {
  return impl->is_valid_position(p);
}

}  // namespace ptzf