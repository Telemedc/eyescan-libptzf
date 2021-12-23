#include "gtest/gtest.h"

#include "utils.h"
#include "position.h"

namespace ptzf {
namespace {

// The fixture for testing class Controller.
class UtilsTest : public ::testing::Test {};

// ensure member order is correct
TEST_F(UtilsTest, TestStringToPosition) {
  auto opt_pos = string_to_position("X:1.00 Y:2.00 Z:3.00 E:4.00 Count X:0 Y:0 Z:0 E:0");
  EXPECT_TRUE(opt_pos);
  if (opt_pos) {
    auto pos = opt_pos.value();
    EXPECT_EQ(pos.x, 1.0);
    EXPECT_EQ(pos.y, 2.0);
    EXPECT_EQ(pos.z, 3.0);
    EXPECT_EQ(pos.f, 4.0);
  }

  auto test_lines = {
    "X:0.00 Y:0.00 Z:0.00 E:0.00 Count X:0 Y:0 Z:0 E:0",
    "X:99.00 Y:99.00 Z:99.00 E:99.00 Count X:0 Y:0 Z:0 E:0"
  };

  for (auto & line : test_lines) {
    opt_pos = string_to_position(line);

    EXPECT_TRUE(opt_pos);
  }
}

}  // namespace
}  // namespace ptzf

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
