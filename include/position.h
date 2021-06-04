#ifndef AC9580AE_6F70_4581_8717_7CFA1101284A
#define AC9580AE_6F70_4581_8717_7CFA1101284A

namespace ptzf {

/**
 * Data structure of a Camera/Printer position.
 */
struct Position {
  /** Pan. */
  double x;
  /** Tilt */
  double y;
  /** Zoom */
  double z;
  /** Focus */
  double f;
  bool is_valid() const;
  static const Position& max();
  static const Position& min();
};

}  // namespace ptzf

#endif /* AC9580AE_6F70_4581_8717_7CFA1101284A */
