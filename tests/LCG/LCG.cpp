#include <gtest/gtest.h>

#include "generators/linear_congruential_engine.hpp"

#include <random>

TEST(LCG, Can_Generator_Correct_Seq_1) {
    std::linear_congruential_engine<std::uint_fast32_t, 16807U, 0U, 2 << 31 - 1> first;
    LinearCongruentialGenerator<std::uint_fast32_t, 16807U, 0U, 2 << 31 - 1> second;
    ASSERT_EQ(first(), second());
}


TEST(LCG, Can_Generator_Correct_Seq_2) {
    std::linear_congruential_engine<std::uint_fast32_t, 16807U, 5U, 2 << 31 - 1> first;
    LinearCongruentialGenerator<std::uint_fast32_t, 16807U, 5U, 2 << 31 - 1> second;
    ASSERT_EQ(first(), second());
}
