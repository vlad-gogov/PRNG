#include <gtest/gtest.h>

#include "generators/linear_congruential_generator.hpp"
#include "generators/base_error.hpp"

#include <random>

TEST(LCG, can_generator_correct_seq_1) {
    constexpr std::uint_fast32_t a = 16807U;
    constexpr std::uint_fast32_t c = 0U;
    constexpr std::uint_fast32_t m = 2 << 31 - 1;
    std::linear_congruential_engine<std::uint_fast32_t, a, c, m> first;
    LinearCongruentialGenerator<std::uint_fast32_t, a, c, m> second;
    ASSERT_EQ(first(), second());
}


TEST(LCG, can_generator_correct_seq_2) {
    constexpr std::uint_fast32_t a = 16807U;
    constexpr std::uint_fast32_t c = 5U;
    constexpr std::uint_fast32_t m = 2 << 31 - 1;
    std::linear_congruential_engine<std::uint_fast32_t, a, c, m> first;
    LinearCongruentialGenerator<std::uint_fast32_t, a, c, m> second;
    ASSERT_EQ(first(), second());
}

TEST(LCG, can_generator_correct_seq_3) {
    constexpr std::uint_fast32_t a = 10U;
    constexpr std::uint_fast32_t c = 5U;
    constexpr std::uint_fast32_t m = 2 << 31 - 1;
    std::linear_congruential_engine<std::uint_fast32_t, a, c, m> first;
    LinearCongruentialGenerator<std::uint_fast32_t, a, c, m> second;
    ASSERT_EQ(first(), second());
}

TEST(LCG, can_throw_exception_modulus) {
    constexpr std::int_fast64_t a = 10;
    constexpr std::int_fast64_t c = 5;
    constexpr std::int_fast64_t m = -1;
    try {
        LinearCongruentialGenerator<std::int_fast64_t, a, c, m> first;
    } catch (const BaseError &error) {
        EXPECT_STREQ(error.what(), "Incorrect modulus");
    }
}

TEST(LCG, can_throw_exception_multiplier_less_zero) {
    constexpr std::int_fast64_t a = -1;
    constexpr std::int_fast64_t c = 5;
    constexpr std::int_fast64_t m = 2 << 16 - 1;
    try {
        LinearCongruentialGenerator<std::int_fast64_t, a, c, m> first;
    } catch (const BaseError &error) {
        EXPECT_STREQ(error.what(), "Incorrect multiplier");
    }
}

TEST(LCG, can_throw_exception_multiplier_more_modulus) {
    constexpr std::int_fast64_t a = 2 << 16 + 1;
    constexpr std::int_fast64_t c = 5;
    constexpr std::int_fast64_t m = 2 << 16 - 1;
    try {
        LinearCongruentialGenerator<std::int_fast64_t, a, c, m> first;
    } catch (const BaseError &error) {
        EXPECT_STREQ(error.what(), "Incorrect multiplier");
    }
}

TEST(LCG, can_throw_exception_increment_less_zero) {
    constexpr std::int_fast64_t a = 10;
    constexpr std::int_fast64_t c = -5;
    constexpr std::int_fast64_t m = 2 << 16 - 1;
    try {
        LinearCongruentialGenerator<std::int_fast64_t, a, c, m> first;
    } catch (const BaseError &error) {
        EXPECT_STREQ(error.what(), "Incorrect increment");
    }
}

TEST(LCG, can_throw_exception_increment_more_modulus) {
    constexpr std::int_fast64_t a = 10;
    constexpr std::int_fast64_t c = 2 << 16 + 1;
    constexpr std::int_fast64_t m = 2 << 16 - 1;
    try {
        LinearCongruentialGenerator<std::int_fast64_t, a, c, m> first;
    } catch (const BaseError &error) {
        EXPECT_STREQ(error.what(), "Incorrect increment");
    }
}

TEST(LCG, can_throw_exception_seed_less_zero) {
    constexpr std::int_fast64_t a = 10;
    constexpr std::int_fast64_t c = 10;
    constexpr std::int_fast64_t m = 2 << 16 - 1;
    try {
        LinearCongruentialGenerator<std::int_fast64_t, a, c, m> first(-1);
    } catch (const BaseError &error) {
        EXPECT_STREQ(error.what(), "Incorrect seed");
    }
}

TEST(LCG, can_throw_exception_seed_more_modules) {
    constexpr std::int_fast64_t a = 10;
    constexpr std::int_fast64_t c = 10;
    constexpr std::int_fast64_t m = 2 << 16 - 1;
    try {
        LinearCongruentialGenerator<std::int_fast64_t, a, c, m> first(2 << 16 + 1);
    } catch (const BaseError &error) {
        EXPECT_STREQ(error.what(), "Incorrect seed");
    }
}

TEST(LCG, can_get_correct_min_if_modulus_not_zero) {
    constexpr std::uint_fast32_t a = 16807U;
    constexpr std::uint_fast32_t c = 5U;
    constexpr std::uint_fast32_t m = 2 << 31 - 1;
    std::linear_congruential_engine<std::uint_fast32_t, a, c, m> first;
    LinearCongruentialGenerator<std::uint_fast32_t, a, c, m> second;
    ASSERT_EQ(first.min(), second.min());
}

TEST(LCG, can_get_correct_min_if_modulus_zero) {
    constexpr std::uint_fast32_t a = 16807U;
    constexpr std::uint_fast32_t c = 0U;
    constexpr std::uint_fast32_t m = 2 << 31 - 1;
    std::linear_congruential_engine<std::uint_fast32_t, a, c, m> first;
    LinearCongruentialGenerator<std::uint_fast32_t, a, c, m> second;
    ASSERT_EQ(first.min(), second.min());
}

TEST(LCG, correct_work_discard_1) {
    constexpr std::uint_fast32_t a = 16807U;
    constexpr std::uint_fast32_t c = 0U;
    constexpr std::uint_fast32_t m = 2 << 31 - 1;
    constexpr std::uint_fast64_t z = 5U;
    std::linear_congruential_engine<std::uint_fast32_t, a, c, m> first;
    LinearCongruentialGenerator<std::uint_fast32_t, a, c, m> second;
    first.discard(z);
    second.discard(z);
    ASSERT_EQ(first(), second());
}

TEST(LCG, correct_work_discard_2) {
    constexpr std::uint_fast32_t a = 16807U;
    constexpr std::uint_fast32_t c = 0U;
    constexpr std::uint_fast32_t m = 2 << 31 - 1;
    constexpr std::uint_fast64_t z = 0U;
    std::linear_congruential_engine<std::uint_fast32_t, a, c, m> first;
    LinearCongruentialGenerator<std::uint_fast32_t, a, c, m> second;
    first.discard(z);
    second.discard(z);
    ASSERT_EQ(first(), second());
}

TEST(LCG, correct_work_discard_3) {
    constexpr std::uint_fast32_t a = 16807U;
    constexpr std::uint_fast32_t c = 0U;
    constexpr std::uint_fast32_t m = 2 << 31 - 1;
    constexpr std::uint_fast64_t z = 100U;
    std::linear_congruential_engine<std::uint_fast32_t, a, c, m> first;
    LinearCongruentialGenerator<std::uint_fast32_t, a, c, m> second;
    first.discard(z);
    second.discard(z);
    ASSERT_EQ(first(), second());
}

TEST(LCG, correct_work_set_seed_1) {
    constexpr std::uint_fast32_t a = 16807U;
    constexpr std::uint_fast32_t c = 0U;
    constexpr std::uint_fast32_t m = 2 << 31 - 1;
    constexpr std::uint_fast32_t seed = 100U;
    std::linear_congruential_engine<std::uint_fast32_t, a, c, m> first;
    LinearCongruentialGenerator<std::uint_fast32_t, a, c, m> second;
    first.seed(seed);
    second.seed(seed);
    ASSERT_EQ(first(), second());
}

TEST(LCG, correct_work_set_seed_2) {
    constexpr std::uint_fast32_t a = 16807U;
    constexpr std::uint_fast32_t c = 0U;
    constexpr std::uint_fast32_t m = 2 << 31 - 1;
    constexpr std::uint_fast32_t seed = 54321U;
    std::linear_congruential_engine<std::uint_fast32_t, a, c, m> first;
    LinearCongruentialGenerator<std::uint_fast32_t, a, c, m> second;
    first.seed(seed);
    second.seed(seed);
    ASSERT_EQ(first(), second());
}
