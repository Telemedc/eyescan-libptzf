#include <sstream>
#include "controller.h"
#include "gtest/gtest.h"

#include "position.h"

namespace ptzf {
namespace {

// The fixture for testing class Controller.
class ControllerConfigTest : public ::testing::Test {};

// ensure Json (de)serialization works
TEST_F(ControllerConfigTest, TestJson) {
  Controller::Config c;
  EXPECT_EQ(c, Controller::Config::from_json(c.to_json()));
}

TEST_F(ControllerConfigTest, TestOperators) {
  // operator ==
  const std::string device("/dev/foo");
  const Position min{-1,-1,-1,-1};
  const Position max{1,1,1,1};
  const bool do_connect = false;
  Controller::Config c{device, min, max, do_connect};
  Controller::Config c2{device, min, max, do_connect};
  Controller::Config c3("/dev/bar", min, max, do_connect);
  EXPECT_EQ(c, c2);
  EXPECT_NE(c, c3);
}

TEST_F(ControllerConfigTest, TestIsValidPosition) {
  // operator ==
  const std::string device("/dev/foo");
  const Position min{-1,-1,-1,-1};
  const Position max{1,1,1,1};
  const bool do_connect = false;
  Controller::Config c{device, min, max, do_connect};

  EXPECT_TRUE(c.is_valid_position(min));
  EXPECT_TRUE(c.is_valid_position(max));
  EXPECT_TRUE(c.is_valid_position(Position()));

  Position x1{1,0,0,0};
  Position y1{0,1,0,0};
  Position z1{0,0,1,0};
  Position f1{0,0,0,1};

  for (const auto offset : {x1, y1, z1, f1}) {
    EXPECT_FALSE(c.is_valid_position(min - offset));
    EXPECT_FALSE(c.is_valid_position(max + offset));
  }
}

}  // namespace
}  // namespace ptzf

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
