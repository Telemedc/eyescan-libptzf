#include "gtest/gtest.h"

#include "position.h"

namespace ptzf {
namespace {

// The fixture for testing class Controller.
class PositionTest : public ::testing::Test {};

// ensure member order is correct
TEST_F(PositionTest, TestMembers) {
  Position p{1,2,3,4};

  EXPECT_EQ(p.x, 1.0);
  EXPECT_EQ(p.y, 2.0);
  EXPECT_EQ(p.z, 3.0);
  EXPECT_EQ(p.f, 4.0);
}

TEST_F(PositionTest, TestJson) {
  Position p{1,2,3,4};
  EXPECT_EQ(p, Position::from_json(p.to_json()));
}

// ensure key based access works (important for Python)
TEST_F(PositionTest, TestKey) {
  Position p{1,2,3,4};

  EXPECT_EQ(p[Position::Key::x], 1.0);
  EXPECT_EQ(p[Position::Key::y], 2.0);
  EXPECT_EQ(p[Position::Key::z], 3.0);
  EXPECT_EQ(p[Position::Key::f], 4.0);
}

// check operator overloading is correct
TEST_F(PositionTest, TestOverloading) {
  Position p{1,2,3,4};

  // operator+
  auto p2 = p + p;

  EXPECT_EQ(p2.x, 2.0);
  EXPECT_EQ(p2.y, 4.0);
  EXPECT_EQ(p2.z, 6.0);
  EXPECT_EQ(p2.f, 8.0);

  // operator*
  auto p3 = p * p;
  EXPECT_EQ(p3.x, 1.0);
  EXPECT_EQ(p3.y, 4.0);
  EXPECT_EQ(p3.z, 9.0);
  EXPECT_EQ(p3.f, 16.0);

  // operator +=
  p += p;
  EXPECT_EQ(p.x, 2.0);
  EXPECT_EQ(p.y, 4.0);
  EXPECT_EQ(p.z, 6.0);
  EXPECT_EQ(p.f, 8.0);

  // operator*=
  p *= p;
  EXPECT_EQ(p.x, 4.0);
  EXPECT_EQ(p.y, 16.0);
  EXPECT_EQ(p.z, 36.0);
  EXPECT_EQ(p.f, 64.0);

  auto p0 = Position{};

  // operator<
  EXPECT_LT(p0, p);

  // operator<=
  EXPECT_LE(p, p);
  EXPECT_LE(p0, p);

  // operator>
  EXPECT_GT(p, p0);

  // operator>=
  EXPECT_GE(p, p);
  EXPECT_GE(p, p0);

  // operator==
  EXPECT_EQ(p, p);
}

}  // namespace
}  // namespace ptzf

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
