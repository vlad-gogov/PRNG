#include <gtest/gtest.h>

TEST(First, sum) {
    float value = 1.0f;
    ASSERT_EQ(value + value, 2.0f);
}
