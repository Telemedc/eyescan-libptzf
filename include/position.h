#ifndef AC9580AE_6F70_4581_8717_7CFA1101284A
#define AC9580AE_6F70_4581_8717_7CFA1101284A

#include <experimental/optional>
#include <string>
#include <valarray>
#include <variant>

namespace ptzf {

/**
 * Data structure of a Camera/Printer position.
 */
struct Position {
  enum class Key {
    x = 1,
    y = 2,
    z = 3,
    f = 4,
  };

  /** Pan. */
  double x;
  /** Tilt */
  double y;
  /** Zoom */
  double z;
  /** Focus */
  double f;

  /** Return a Position from json string. Can throw. */
  static Position from_json(const std::string& s);
  std::string to_json() const;

  /**
   * Return a Position from gcode string (M114 return)
   * (or nullopt if the string could not be parsed)
   */
  static std::experimental::optional<Position> from_gcode(
      const std::string& line);
  std::string to_gcode() const;

  // This exists mostly as a python __getitem__
  double operator[](Key key) const {
    switch (key) {
      // clang-format off
      case (Key::x): { return x; }
      case (Key::y): { return y; }
      case (Key::z): { return z; }
      case (Key::f): { return f; }
      // clang-format on
      default:
        // logic error - new enum is not handled above
        std::abort();
    }
  }
  Position operator+(const Position& o) const {
    return {x + o.x, y + o.y, z + o.z, f + o.f};
  }
  Position operator-(const Position& o) const {
    return {x - o.x, y - o.y, z - o.z, f - o.f};
  }
  Position operator*(const Position& o) const {
    return {x * o.x, y * o.y, z * o.z, f * o.f};
  }
  Position& operator+=(const Position& o) {
    x += o.x;
    y += o.y;
    z += o.z;
    f += o.f;
    return *this;
  }
  Position& operator-=(const Position& o) {
    x -= o.x;
    y -= o.y;
    z -= o.z;
    f -= o.f;
    return *this;
  }
  Position& operator*=(const Position& o) {
    x *= o.x;
    y *= o.y;
    z *= o.z;
    f *= o.f;
    return *this;
  }
  bool operator<(const Position& o) const {
    return x < o.x && y < o.y && z < o.z && f < o.f;
  }
  bool operator<=(const Position& o) const {
    return x <= o.x && y <= o.y && z <= o.z && f <= o.f;
  }
  bool operator>(const Position& o) const {
    return x > o.x && y > o.y && z > o.z && f > o.f;
  }
  bool operator>=(const Position& o) const {
    return x >= o.x && y >= o.y && z >= o.z && f >= o.f;
  }
  bool operator==(const Position& o) const {
    return x == o.x && y == o.y && z == o.z && f == o.f;
  }
  bool operator!=(const Position& o) const { return !(*this == o); }
};

}  // namespace ptzf

#endif /* AC9580AE_6F70_4581_8717_7CFA1101284A */
