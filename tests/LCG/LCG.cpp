#include <gtest/gtest.h>

#include "generators/linear_congruential_generator.hpp"

#include <random>

TEST(LCG, can_generator_correct_seq_1) {
    std::linear_congruential_engine<std::uint_fast32_t, 16807U, 0U, 2 << 31 - 1> first;
    LinearCongruentialGenerator<std::uint_fast32_t, 16807U, 0U, 2 << 31 - 1> second;
    ASSERT_EQ(first(), second());
}


TEST(LCG, can_generator_correct_seq_2) {
    std::linear_congruential_engine<std::uint_fast32_t, 16807U, 5U, 2 << 31 - 1> first;
    LinearCongruentialGenerator<std::uint_fast32_t, 16807U, 5U, 2 << 31 - 1> second;
    ASSERT_EQ(first(), second());
}

TEST(LCG, can_throw_modulus_exception) {
    try {
        LinearCongruentialGenerator<std::int64_t, 16807U, 5U, -1> first;
    } catch (const char *str) {
        EXPECT_STREQ(str, "Incorrect modulus");
    }
}
