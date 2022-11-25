#include <gtest/gtest.h>

#include "generators/base_error.hpp"
#include "generators/linear_congruential_generator.hpp"

#include <random>

TEST(LCG, can_generator_correct_seq_1) {
    constexpr std::uint_fast32_t a = 16807U;
    constexpr std::uint_fast32_t c = 0U;
    constexpr std::uint_fast32_t m = 2 << 31 - 1;
    std::linear_congruential_engine<std::uint_fast32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint_fast32_t, a, c, m> our_generator;
    ASSERT_EQ(correct_generator(), our_generator());
}

TEST(LCG, can_generator_correct_seq_2) {
    constexpr std::uint_fast32_t a = 16807U;
    constexpr std::uint_fast32_t c = 5U;
    constexpr std::uint_fast32_t m = 2 << 31 - 1;
    std::linear_congruential_engine<std::uint_fast32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint_fast32_t, a, c, m> our_generator;
    ASSERT_EQ(correct_generator(), our_generator());
}

TEST(LCG, can_generator_correct_seq_3) {
    constexpr std::uint_fast32_t a = 10U;
    constexpr std::uint_fast32_t c = 5U;
    constexpr std::uint_fast32_t m = 2 << 31 - 1;
    std::linear_congruential_engine<std::uint_fast32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint_fast32_t, a, c, m> our_generator;
    ASSERT_EQ(correct_generator(), our_generator());
}

TEST(LCG, can_throw_exception_incorrect_type_int) {
    constexpr std::int_fast64_t a = 10;
    constexpr std::int_fast64_t c = 5;
    constexpr std::int_fast64_t m = -1;
    try {
        LinearCongruentialGenerator<std::int_fast64_t, a, c, m> correct_generator;
    } catch (const BaseError &error) {
        EXPECT_STREQ(error.what(), "LCG can work with unsigned integers");
    }
}

TEST(LCG, can_throw_exception_incorrect_type_char) {
    constexpr char32_t a = 10;
    constexpr char32_t c = 5;
    constexpr char32_t m = -1;
    try {
        LinearCongruentialGenerator<char32_t, a, c, m> correct_generator;
    } catch (const BaseError &error) {
        EXPECT_STREQ(error.what(), "LCG can work with unsigned integers");
    }
}

TEST(LCG, can_throw_exception_incorrect_type_uchart) {
    constexpr std::uint_fast8_t a = 10U;
    constexpr std::uint_fast8_t c = 5U;
    constexpr std::uint_fast8_t m = 128U;
    try {
        LinearCongruentialGenerator<uint_fast8_t, a, c, m> correct_generator;
    } catch (const BaseError &error) {
        FAIL();
    }
    SUCCEED();
}

TEST(LCG, can_throw_exception_incorrect_type_ushort) {
    constexpr std::uint_fast16_t a = 10U;
    constexpr std::uint_fast16_t c = 5U;
    constexpr std::uint_fast16_t m = 2 << 31 - 1;
    try {
        LinearCongruentialGenerator<uint_fast16_t, a, c, m> correct_generator;
    } catch (const BaseError &error) {
        FAIL();
    }
    SUCCEED();
}

TEST(LCG, can_throw_exception_incorrect_type_ulong) {
    constexpr std::uint_fast32_t a = 10U;
    constexpr std::uint_fast32_t c = 5U;
    constexpr std::uint_fast32_t m = 2 << 31 - 1;
    try {
        LinearCongruentialGenerator<uint_fast32_t, a, c, m> correct_generator;
    } catch (const BaseError &error) {
        FAIL();
    }
    SUCCEED();
}

TEST(LCG, can_throw_exception_incorrect_type_ulonglong) {
    constexpr std::uint_fast64_t a = 10U;
    constexpr std::uint_fast64_t c = 5U;
    constexpr std::uint_fast64_t m = 2 << 31 - 1;
    try {
        LinearCongruentialGenerator<uint_fast64_t, a, c, m> correct_generator;
    } catch (const BaseError &error) {
        FAIL();
    }
    SUCCEED();
}

TEST(LCG, can_throw_exception_incorrect_type_6) {
    constexpr std::uint_fast16_t a = 10U;
    constexpr std::uint_fast16_t c = 5U;
    constexpr std::uint_fast16_t m = 2 << 31 - 1;
    try {
        LinearCongruentialGenerator<uint_fast16_t, a, c, m> correct_generator;
    } catch (const BaseError &error) {
        FAIL();
    }
    SUCCEED();
}

TEST(LCG, can_throw_exception_modulus) {
    constexpr std::uint_fast64_t a = 10;
    constexpr std::uint_fast64_t c = 5;
    constexpr std::uint_fast64_t m = -1;
    try {
        LinearCongruentialGenerator<std::uint_fast64_t, a, c, m> correct_generator;
    } catch (const BaseError &error) {
        EXPECT_STREQ(error.what(), "Incorrect modulus");
    }
}

TEST(LCG, can_throw_exception_multiplier_more_modulus) {
    constexpr std::uint_fast64_t a = 2 << 16 + 1;
    constexpr std::uint_fast64_t c = 5;
    constexpr std::uint_fast64_t m = 2 << 16 - 1;
    try {
        LinearCongruentialGenerator<std::uint_fast64_t, a, c, m> correct_generator;
    } catch (const BaseError &error) {
        EXPECT_STREQ(error.what(), "Incorrect multiplier");
    }
}

TEST(LCG, can_throw_exception_increment_more_modulus) {
    constexpr std::uint_fast64_t a = 10;
    constexpr std::uint_fast64_t c = 2 << 16 + 1;
    constexpr std::uint_fast64_t m = 2 << 16 - 1;
    try {
        LinearCongruentialGenerator<std::uint_fast64_t, a, c, m> correct_generator;
    } catch (const BaseError &error) {
        EXPECT_STREQ(error.what(), "Incorrect increment");
    }
}

TEST(LCG, can_throw_exception_seed_more_modules) {
    constexpr std::uint_fast64_t a = 10;
    constexpr std::uint_fast64_t c = 10;
    constexpr std::uint_fast64_t m = 2 << 16 - 1;
    try {
        LinearCongruentialGenerator<std::uint_fast64_t, a, c, m> correct_generator(2 << 16 + 1);
    } catch (const BaseError &error) {
        EXPECT_STREQ(error.what(), "Incorrect seed");
    }
}

TEST(LCG, can_get_correct_min_if_modulus_not_zero) {
    constexpr std::uint_fast32_t a = 16807U;
    constexpr std::uint_fast32_t c = 5U;
    constexpr std::uint_fast32_t m = 2 << 31 - 1;
    std::linear_congruential_engine<std::uint_fast32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint_fast32_t, a, c, m> our_generator;
    ASSERT_EQ(correct_generator.min(), our_generator.min());
}

TEST(LCG, can_get_correct_min_if_modulus_zero) {
    constexpr std::uint_fast32_t a = 16807U;
    constexpr std::uint_fast32_t c = 0U;
    constexpr std::uint_fast32_t m = 2 << 31 - 1;
    std::linear_congruential_engine<std::uint_fast32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint_fast32_t, a, c, m> our_generator;
    ASSERT_EQ(correct_generator.min(), our_generator.min());
}

TEST(LCG, correct_work_discard_1) {
    constexpr std::uint_fast32_t a = 16807U;
    constexpr std::uint_fast32_t c = 0U;
    constexpr std::uint_fast32_t m = 2 << 31 - 1;
    constexpr std::uint_fast32_t z = 5U;
    std::linear_congruential_engine<std::uint_fast32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint_fast32_t, a, c, m> our_generator;
    correct_generator.discard(z);
    our_generator.discard(z);
    ASSERT_EQ(correct_generator(), our_generator());
}

TEST(LCG, correct_work_discard_2) {
    constexpr std::uint_fast32_t a = 16807U;
    constexpr std::uint_fast32_t c = 0U;
    constexpr std::uint_fast32_t m = 2 << 31 - 1;
    constexpr std::uint_fast32_t z = 0U;
    std::linear_congruential_engine<std::uint_fast32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint_fast32_t, a, c, m> our_generator;
    correct_generator.discard(z);
    our_generator.discard(z);
    ASSERT_EQ(correct_generator(), our_generator());
}

TEST(LCG, correct_work_discard_3) {
    constexpr std::uint_fast32_t a = 16807U;
    constexpr std::uint_fast32_t c = 0U;
    constexpr std::uint_fast32_t m = 2 << 31 - 1;
    constexpr std::uint_fast32_t z = 100U;
    std::linear_congruential_engine<std::uint_fast32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint_fast32_t, a, c, m> our_generator;
    correct_generator.discard(z);
    our_generator.discard(z);
    ASSERT_EQ(correct_generator(), our_generator());
}

TEST(LCG, correct_work_set_seed_1) {
    constexpr std::uint_fast32_t a = 16807U;
    constexpr std::uint_fast32_t c = 0U;
    constexpr std::uint_fast32_t m = 2 << 31 - 1;
    constexpr std::uint_fast32_t seed = 100U;
    std::linear_congruential_engine<std::uint_fast32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint_fast32_t, a, c, m> our_generator;
    correct_generator.seed(seed);
    our_generator.seed(seed);
    ASSERT_EQ(correct_generator(), our_generator());
}

TEST(LCG, correct_work_set_seed_2) {
    constexpr std::uint_fast32_t a = 16807U;
    constexpr std::uint_fast32_t c = 0U;
    constexpr std::uint_fast32_t m = 2 << 31 - 1;
    constexpr std::uint_fast32_t seed = 54321U;
    std::linear_congruential_engine<std::uint_fast32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint_fast32_t, a, c, m> our_generator;
    correct_generator.seed(seed);
    our_generator.seed(seed);
    ASSERT_EQ(correct_generator(), our_generator());
}
