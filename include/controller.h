#ifndef C540D9D8_4FC5_4163_BA5C_7C2BB87EFD0E
#define C540D9D8_4FC5_4163_BA5C_7C2BB87EFD0E

#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include "position.h"

namespace ptzf {

/**
 * A class for controlling Pan, Tilt, Zoom and Focus.
 */
class Controller final {
 public:
  /**
   * @brief Construct a new Controller object
   *
   * @param device path to a printer serial device
   * @param do_connect whether to connect on construct. if false, you must
   * `connect()` manually before you `go()` to a `Position`.
   */
  Controller(std::string device, bool do_connect = true);
  ~Controller();

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
  bool go(Position p);

 private:
  struct Impl;
  std::unique_ptr<Impl> impl;
};

}  // namespace ptzf

#endif /* C540D9D8_4FC5_4163_BA5C_7C2BB87EFD0E */