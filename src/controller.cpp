#include "controller.h"
#include "utils.h"

#include <chrono>
#include <iostream>

namespace ptzf {

Controller::Controller(std::string device, bool do_connect)
    : device(device), stream() {
  if (do_connect) {
    this->connect();
  }
}

bool Controller::connect() {
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

bool Controller::disconnect() {
  // this signals all threads to stop
  if (this->stream.IsOpen()) {
    LOG(debug) << "Closing: " << this->device;
    this->stream.Close();
  }
  LOG(debug) << "Closed: " << this->device;
  return true;
}

bool Controller::is_connected() const {
  return this->stream.IsOpen();
}

bool Controller::go(Position p) {
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

}  // namespace ptzf