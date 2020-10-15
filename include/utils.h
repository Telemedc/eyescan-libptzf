#ifndef C083A083_8E43_4310_9FAA_11E0CEB64D65
#define C083A083_8E43_4310_9FAA_11E0CEB64D65

#include "position.h"

#include <experimental/optional>
#include <thread>

#include <boost/log/trivial.hpp>

using std::experimental::optional;

// our logging macro
#define LOG BOOST_LOG_TRIVIAL

namespace ptzf {

/**
 * Joins a thread and LOG to debug level.
 */
void join_and_log(std::thread& t);

/**
 * Parse a line of GCode and return a Position if possible
 */
optional<Position> string_to_position(const std::string& line);

/**
 * Convert a position to string.
 */
std::string position_to_string(const Position& p);

}  // namespace ptzf

#endif /* C083A083_8E43_4310_9FAA_11E0CEB64D65 */
