#include "gtest/gtest.h"

#include "position.h"

namespace ptzf {
namespace {

// The fixture for testing class Controller.
class PositionTest : public ::testing::Test {};

// ensures is_valid works so positions can't be out of range
// this could potentially break hardware, so it's pretty important
TEST_F(PositionTest, TestIsValid) {
  Position too_big{
      .x = Position::max().x + 1.0,
      .y = Position::max().y + 1.0,
      .z = Position::max().z + 1.0,
      .f = Position::max().f + 1.0,
  };

  Position not_too_big{
      .x = Position::max().x - 1.0,
      .y = Position::max().y - 1.0,
      .z = Position::max().z - 1.0,
      .f = Position::max().f - 1.0,
  };

  Position too_small{
      .x = Position::min().x - 1.0,
      .y = Position::min().y - 1.0,
      .z = Position::min().z - 1.0,
      .f = Position::min().f - 1.0,
  };

  Position not_too_small{
      .x = Position::min().x + 1.0,
      .y = Position::min().y + 1.0,
      .z = Position::min().z + 1.0,
      .f = Position::min().f + 1.0,
  };

  EXPECT_FALSE(too_big.is_valid());
  EXPECT_TRUE(not_too_big.is_valid());
  EXPECT_FALSE(too_small.is_valid());
  EXPECT_TRUE(not_too_small.is_valid());
}

}  // namespace
}  // namespace ptzf

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
