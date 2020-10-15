#ifndef AC9580AE_6F70_4581_8717_7CFA1101284A
#define AC9580AE_6F70_4581_8717_7CFA1101284A

namespace ptzf {

/**
 * Data structure of a Camera/Printer position.
 */
struct Position {
  /** Pan. */
  float x;
  /** Tilt */
  float y;
  /** Zoom */
  float z;
  /** Focus */
  float f;
};

}  // namespace ptzf

#endif /* AC9580AE_6F70_4581_8717_7CFA1101284A */
