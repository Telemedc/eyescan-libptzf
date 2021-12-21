#ifndef FD203923_0D8F_425B_90AE_EA860E715CD7
#define FD203923_0D8F_425B_90AE_EA860E715CD7

#include <experimental/optional>

#include "position.h"
#ifdef LIBSERIAL_GE_1_0_0
#include <libserial/SerialStream.h>
#else
#include <SerialStream.h>
#endif

namespace ptzf {

/**
 * cstring to listen for when go destination is reached.
 */
const char OK_CODE[] = "ok P63 B31";

/**
 * code to indicate printer is busy
 */
const char BUSY_CODE[] = "echo:busy:";

/**
 * cstring to listen for when error occurs.
 */
const char ERROR_CODE[] = "error";

/**
 * wait for ok (or error) from a stream
 */
bool wait_for_ok(LibSerial::SerialStream& stream);

const std::experimental::optional<Position>
get_minimum_limit(LibSerial::SerialStream &stream);

const std::experimental::optional<Position>
get_maximum_limit(LibSerial::SerialStream &stream);

}  // namespace ptzf

#endif /* FD203923_0D8F_425B_90AE_EA860E715CD7 */
