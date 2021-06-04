#include "position.h"
#include "ptzf_config.h"

namespace ptzf {

constexpr Position MAX_POS {
  .x = MAX_POS_X,
  .y = MAX_POS_Y,
  .z = MAX_POS_Z,
  .f = MAX_POS_F,
};

constexpr Position MIN_POS {
  .x = MIN_POS_X,
  .y = MIN_POS_Y,
  .z = MIN_POS_Z,
  .f = MIN_POS_F,
};

const Position& Position::max() {
  return MAX_POS;
}

const Position& Position::min() {
  return MIN_POS;
}

bool Position::is_valid() const {
  return (x <= MAX_POS_X && x >= MIN_POS_X) &&
         (y <= MAX_POS_Y && y >= MIN_POS_Y) &&
         (z <= MAX_POS_Z && z >= MIN_POS_Z) &&
         (f <= MAX_POS_F && f >= MIN_POS_F);
}

}  // namespace ptzf
