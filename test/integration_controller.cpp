#include "controller.h"
#include "utils.h"

#include "gtest/gtest.h"

#include <experimental/optional>

// clang-format off
#ifndef PRINTER
  #define PRINTER "/dev/simulated_printer"
#endif
// clang-format on

namespace ptzf {
namespace {

// The fixture for testing class Controller.
class ControllerTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  ControllerTest() {}

  ~ControllerTest() override {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  void TearDown() override {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  // Class members declared here can be used by all tests in the test suite
  // for Foo.
};

// Tests that the Foo::Bar() method does Abc.
TEST_F(ControllerTest, SlowTest) {
  Controller c(PRINTER);
  // @lackdaz you can add/remove positions to tests here
  auto max = Position::max();
  auto min = Position::min();
  Position positions[] = {
      Position{max.x, min.y, min.z, min.f},
      Position{max.x, min.y, min.z, min.f},
      Position{max.x, max.y, min.z, min.f},
      Position{max.x, min.y, min.z, min.f},
      Position{max.x, max.y, min.z, min.f},
      Position{max.x, min.y, min.z, min.f},
      Position{max.x, max.y, min.z, min.f},
      Position{max.x, max.y, min.z, min.f},
      Position{max.x, max.y, min.z, min.f},
      Position{max.x, min.y, min.z, min.f},
  };

  for (const auto& p : positions) {
    ASSERT_TRUE(c.go(p));
  }
}

}  // namespace
}  // namespace ptzf

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
