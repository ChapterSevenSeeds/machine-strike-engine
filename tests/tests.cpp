#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(machine_strike_engine_test, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "hello");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}