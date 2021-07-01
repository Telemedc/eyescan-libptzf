#ifndef C083A083_8E43_4310_9FAA_11E0CEB64D65
#define C083A083_8E43_4310_9FAA_11E0CEB64D65

#include <experimental/optional>

#include "position.h"

namespace ptzf {

/**
 * Parse a line of GCode and return a Position if possible
 */
std::experimental::optional<Position> string_to_position(
    const std::string& line);

/**
 * Convert a position to string.
 */
std::string position_to_string(const Position& p);

}  // namespace ptzf

#endif /* C083A083_8E43_4310_9FAA_11E0CEB64D65 */
