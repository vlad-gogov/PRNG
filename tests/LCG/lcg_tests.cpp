#include <gtest/gtest.h>

#include "generators/base_error.hpp"
#include "generators/linear_congruential_generator.hpp"

#include <iostream>
#include <limits>
#include <random>

TEST(Lcg, can_generator_correct_seq_1) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    std::linear_congruential_engine<std::uint32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint32_t, a, c, m> our_generator;
    ASSERT_EQ(correct_generator(), our_generator());
}

TEST(Lcg, can_generator_correct_seq_2) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    std::linear_congruential_engine<std::uint32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint32_t, a, c, m> our_generator;
    std::uint32_t count_number = 5;
    std::vector<std::uint32_t> correct_number;
    std::vector<std::uint32_t> our_number;
    correct_number.reserve(count_number);
    our_number.reserve(count_number);
    for (std::uint32_t i = 0; i < count_number; ++i) {
        correct_number.push_back(correct_generator());
        our_number.push_back(our_generator());
    }
    ASSERT_EQ(correct_number, our_number);
}

TEST(Lcg, can_generator_correct_seq_3) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    std::linear_congruential_engine<std::uint32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint32_t, a, c, m> our_generator;
    std::uint32_t count_number = 10;
    std::vector<std::uint32_t> correct_number;
    std::vector<std::uint32_t> our_number;
    correct_number.reserve(count_number);
    our_number.reserve(count_number);
    for (std::uint32_t i = 0; i < count_number; ++i) {
        correct_number.push_back(correct_generator());
        our_number.push_back(our_generator());
    }
    ASSERT_EQ(correct_number, our_number);
}

TEST(Lcg, can_generator_correct_seq_4) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 5U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    std::linear_congruential_engine<std::uint32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint32_t, a, c, m> our_generator;
    ASSERT_EQ(correct_generator(), our_generator());
}

TEST(Lcg, can_generator_correct_seq_5) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 5U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    std::linear_congruential_engine<std::uint32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint32_t, a, c, m> our_generator;
    std::uint32_t count_number = 5;
    std::vector<std::uint32_t> correct_number;
    std::vector<std::uint32_t> our_number;
    correct_number.reserve(count_number);
    our_number.reserve(count_number);
    for (std::uint32_t i = 0; i < count_number; ++i) {
        correct_number.push_back(correct_generator());
        our_number.push_back(our_generator());
    }
    ASSERT_EQ(correct_number, our_number);
}

TEST(Lcg, can_generator_correct_seq_6) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 5U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    std::linear_congruential_engine<std::uint32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint32_t, a, c, m> our_generator;
    std::uint32_t count_number = 15;
    std::vector<std::uint32_t> correct_number;
    std::vector<std::uint32_t> our_number;
    correct_number.reserve(count_number);
    our_number.reserve(count_number);
    for (std::uint32_t i = 0; i < count_number; ++i) {
        correct_number.push_back(correct_generator());
        our_number.push_back(our_generator());
    }
    ASSERT_EQ(correct_number, our_number);
}

TEST(Lcg, can_generator_correct_seq_7) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 5U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    std::linear_congruential_engine<std::uint32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint32_t, a, c, m> our_generator;
    std::uint32_t count_number = 200;
    std::vector<std::uint32_t> correct_number;
    std::vector<std::uint32_t> our_number;
    correct_number.reserve(count_number);
    our_number.reserve(count_number);
    for (std::uint32_t i = 0; i < count_number; ++i) {
        correct_number.push_back(correct_generator());
        our_number.push_back(our_generator());
    }
    ASSERT_EQ(correct_number, our_number);
}

TEST(Lcg, can_throw_exception_seed_more_modules) {
    constexpr std::uint64_t a = 10U;
    constexpr std::uint64_t c = 10U;
    constexpr std::uint64_t m = (2U << 16U) - 1U;
    try {
        LinearCongruentialGenerator<std::uint64_t, a, c, m> correct_generator(m + 2U);
    } catch (const BaseError &error) {
        ASSERT_STREQ(error.what(), "Incorrect seed");
    }
}

TEST(Lcg, can_get_correct_min_if_modulus_not_zero) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 5U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    std::linear_congruential_engine<std::uint32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint32_t, a, c, m> our_generator;
    ASSERT_EQ(correct_generator.min(), our_generator.min());
}

TEST(Lcg, can_get_correct_min_if_modulus_zero) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    std::linear_congruential_engine<std::uint32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint32_t, a, c, m> our_generator;
    ASSERT_EQ(correct_generator.min(), our_generator.min());
}

TEST(Lcg, correct_work_discard_1) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    constexpr std::uint32_t z = 5U;
    std::linear_congruential_engine<std::uint32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint32_t, a, c, m> our_generator;
    correct_generator.discard(z);
    our_generator.discard(z);
    ASSERT_EQ(correct_generator(), our_generator());
}

TEST(Lcg, correct_work_discard_2) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    constexpr std::uint32_t z = 0U;
    std::linear_congruential_engine<std::uint32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint32_t, a, c, m> our_generator;
    correct_generator.discard(z);
    our_generator.discard(z);
    ASSERT_EQ(correct_generator(), our_generator());
}

TEST(Lcg, correct_work_discard_3) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    constexpr std::uint32_t z = 10U;
    std::linear_congruential_engine<std::uint32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint32_t, a, c, m> our_generator;
    correct_generator.discard(z);
    our_generator.discard(z);
    ASSERT_EQ(correct_generator(), our_generator());
}

TEST(Lcg, correct_work_set_seed_1) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    constexpr std::uint32_t seed = 100U;
    std::linear_congruential_engine<std::uint32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint32_t, a, c, m> our_generator;
    correct_generator.seed(seed);
    our_generator.seed(seed);
    ASSERT_EQ(correct_generator(), our_generator());
}

TEST(Lcg, correct_work_set_seed_2) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    constexpr std::uint32_t seed = 54321U;
    std::linear_congruential_engine<std::uint32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint32_t, a, c, m> our_generator;
    correct_generator.seed(seed);
    our_generator.seed(seed);
    ASSERT_EQ(correct_generator(), our_generator());
}
