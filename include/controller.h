#ifndef C540D9D8_4FC5_4163_BA5C_7C2BB87EFD0E
#define C540D9D8_4FC5_4163_BA5C_7C2BB87EFD0E

#include <mutex>
#include <string>
#include <thread>

#include <SerialStream.h>

#include "utils.h"

using std::experimental::optional;

namespace ptzf {

/**
 * A class for controlling Pan, Tilt, Zoom and Focus.
 */
class Controller {
 private:
  /** LibSerial::SerialStream for the device */
  LibSerial::SerialStream stream;

 public:
  Controller(std::string device)
      : stream(device, std::ios::in | std::ios::out){};
  ~Controller() = default;

  /**
   * Go to a specific postion. Block until there.
   *
   * @return false on fail, true on success
   */
  bool go(const Position& p);
};

}  // namespace ptzf

#endif /* C540D9D8_4FC5_4163_BA5C_7C2BB87EFD0E */