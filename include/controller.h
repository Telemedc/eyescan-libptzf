#ifndef C540D9D8_4FC5_4163_BA5C_7C2BB87EFD0E
#define C540D9D8_4FC5_4163_BA5C_7C2BB87EFD0E

#include <memory>
#include <string>
#include <experimental/optional>

#include "position.h"

namespace ptzf {

constexpr auto DEFAULT_DEVICE = "/dev/MCB";
constexpr Position DEFAULT_MIN_POSITION = {0,0,0,0};
constexpr Position DEFAULT_MAX_POSITION = {200,200,200,200};
constexpr bool DEFAULT_DO_CONNECT = true;

/**
 * A class for controlling Pan, Tilt, Zoom and Focus.
 */
class Controller final {
 public:
  struct Config final {
    const std::string device = DEFAULT_DEVICE;
    const Position min = DEFAULT_MIN_POSITION;
    const Position max = DEFAULT_MAX_POSITION;
    const bool do_connect = DEFAULT_DO_CONNECT;

    Config() {}
    Config(std::string device, Position p_min, Position p_max, bool do_connect)
      : device(device), min(p_min), max(p_max), do_connect(do_connect) {}
    Config(std::string device, bool do_connect)
      : device(device), do_connect(do_connect) {}

    bool operator==(const Config& o) const {
      return (
        device == o.device
        && min == o.min
        && max == o.max
        && do_connect == o.do_connect
      );
    }
    bool operator !=(const Config& o) const {
      return !(*this == o);
    }

    /**
     * @brief Deserialize Config from json string
     */
    static Config from_json(const std::string& s);
    /**
     * @brief Serialize to json string
     */
    std::string to_json() const;

    /** Checks every value (`v`) in `p` is between `min.v` and `max.v` **/
    bool is_valid_position(Position p) const {
      return p >= min && p <= max;
    }
  };

  /**
   * @brief Construct a new Controller object
   *
   * @param device path to a printer serial device
   * @param do_connect whether to connect on construct. if false, you must
   * `connect()` manually before you `go()` to a `Position`.
   */
  Controller(std::string device, bool do_connect = true);
  Controller(Config&& conf);
  ~Controller();

  /**
   * @brief Do auto-home (G28). Block until complete.
   * 
   * @return false on failure
   */
  bool go_home();

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
   * @brief Get the config for this Controller
   * (to modify, create a new Controller).
   */
  const Config& get_config() const;

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