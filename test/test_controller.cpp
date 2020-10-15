#include "controller.h"
#include "gtest/gtest.h"

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>

namespace ptzf {
namespace {

// The fixture for testing class Controller.
class ControllerTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  ControllerTest() {
    boost::log::core::get()->set_filter(boost::log::trivial::severity >=
                                        boost::log::trivial::debug);
  }

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
TEST_F(ControllerTest, InteractiveTest) {
  Controller c("/dev/SKR");
  // TODO(mdegans): figure out extents
  Position min_values{0.0f, 0.0f, 0.0f, 0.0f};
  Position max_values{96.0f, 42.0f, 47.0f, 12.0f};

  ASSERT_TRUE(c.go(min_values));
  ASSERT_TRUE(c.go(max_values));
  ASSERT_TRUE(c.go(min_values));
  ASSERT_TRUE(c.go(max_values));
  ASSERT_TRUE(c.go(min_values));
}

}  // namespace
}  // namespace ptzf

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
