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
class ControllerStressTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  ControllerStressTest() {}

  ~ControllerStressTest() override {
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

// Tests that the controller doesn't segfault after being created and destroyed
// repeatedly, which is an issue found when testing with ptzf.
TEST_F(ControllerStressTest, CreateDestroyLoop) {
  // this probably will only fail with a real printer
  for (int i = 0; i < 10; i++) {
    Controller ctrl(PRINTER);
    (void)ctrl;
  }
}

}  // namespace
}  // namespace ptzf

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
