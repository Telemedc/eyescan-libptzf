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
  /** The device name */
  std::string device;
  /** LibSerial::SerialStream for the device */
  LibSerial::SerialStream stream;

 public:
  Controller(std::string device, bool do_connect = true);
  ~Controller() = default;

  /**
   * Connect to the printer/camera.
   *
   * @return false on failure
   */
  bool connect();

  /**
   * Disconnect from the printer/camera.
   *
   * @return false on failure
   */
  bool disconnect();

  /**
   * @return true if printer/camera is connected.
   * 
   * Note: currently this cannot fail. It is not required to disconnect
   * normally. The internal stream destructor should do that.
   */
  bool is_connected() const;

  /**
   * Go to a specific postion. Block until there.
   *
   * @return false on fail, true on success
   */
  bool go(const Position& p);
};

}  // namespace ptzf

#endif /* C540D9D8_4FC5_4163_BA5C_7C2BB87EFD0E */