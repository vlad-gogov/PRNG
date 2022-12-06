#include <gtest/gtest.h>

#include "generators/base_error.hpp"
#include "generators/lagged_fibonacci_generator.hpp"

#include <iostream>
#include <limits>
#include <random>

TEST(LFG, can_generator_correct_seq_1) {
    constexpr std::uint_fast32_t j = 7U;
    constexpr std::uint_fast32_t k = 10U;
    constexpr std::uint_fast32_t m = std::numeric_limits<std::uint_fast32_t>::digits;
    std::subtract_with_carry_engine<std::uint_fast32_t, m, j, k> correct_generator;
    LaggedFibonacciGenerator<std::uint_fast32_t, m, j, k> our_generator;
    ASSERT_EQ(correct_generator(), our_generator());
    
}

TEST(LFG, can_generator_correct_seq_2) {
    constexpr std::uint_fast32_t j = 7U;
    constexpr std::uint_fast32_t k = 10U;
    constexpr std::uint_fast32_t m = std::numeric_limits<std::uint_fast32_t>::digits;
    std::subtract_with_carry_engine<std::uint_fast32_t, m, j, k> correct_generator;
    LaggedFibonacciGenerator<std::uint_fast32_t, m, j, k> our_generator;
    std::uint_fast32_t count_number = 5;
    std::vector<std::uint_fast32_t> correct_number;
    std::vector<std::uint_fast32_t> our_number;
    correct_number.reserve(count_number);
    our_number.reserve(count_number);
    for (std::uint_fast32_t i = 0; i < count_number; ++i) {
        correct_number.push_back(correct_generator());
        our_number.push_back(our_generator());
    }
    ASSERT_EQ(correct_number, our_number);
}

TEST(LFG, can_generator_correct_seq_3) {
    constexpr std::uint_fast32_t j = 7U;
    constexpr std::uint_fast32_t k = 10U;
    constexpr std::uint_fast32_t m = std::numeric_limits<std::uint_fast32_t>::digits;
    std::subtract_with_carry_engine<std::uint_fast32_t, m, j, k> correct_generator;
    LaggedFibonacciGenerator<std::uint_fast32_t, m, j, k> our_generator;
    std::uint_fast32_t count_number = 10;
    std::vector<std::uint_fast32_t> correct_number;
    std::vector<std::uint_fast32_t> our_number;
    correct_number.reserve(count_number);
    our_number.reserve(count_number);
    for (std::uint_fast32_t i = 0; i < count_number; ++i) {
        correct_number.push_back(correct_generator());
        our_number.push_back(our_generator());
    }
    ASSERT_EQ(correct_number, our_number);
}

TEST(LFG, can_generator_correct_seq_4) {
    constexpr std::uint_fast32_t j = 7U;
    constexpr std::uint_fast32_t k = 10U;
    constexpr std::uint_fast32_t m = std::numeric_limits<std::uint_fast32_t>::digits;
    std::subtract_with_carry_engine<std::uint_fast32_t, m, j, k> correct_generator;
    LaggedFibonacciGenerator<std::uint_fast32_t, m, j, k> our_generator;
    ASSERT_EQ(correct_generator(), our_generator());
}

TEST(LFG, can_generator_correct_seq_5) {
    constexpr std::uint_fast32_t j = 7U;
    constexpr std::uint_fast32_t k = 10U;
    constexpr std::uint_fast32_t m = std::numeric_limits<std::uint_fast32_t>::digits;
    std::subtract_with_carry_engine<std::uint_fast32_t, m, j, k> correct_generator;
    LaggedFibonacciGenerator<std::uint_fast32_t, m, j, k> our_generator;
    std::uint_fast32_t count_number = 5;
    std::vector<std::uint_fast32_t> correct_number;
    std::vector<std::uint_fast32_t> our_number;
    correct_number.reserve(count_number);
    our_number.reserve(count_number);
    for (std::uint_fast32_t i = 0; i < count_number; ++i) {
        correct_number.push_back(correct_generator());
        our_number.push_back(our_generator());
    }
    ASSERT_EQ(correct_number, our_number);
}

TEST(LFG, can_generator_correct_seq_6) {
    constexpr std::uint_fast32_t j = 7U;
    constexpr std::uint_fast32_t k = 10U;
    constexpr std::uint_fast32_t m = std::numeric_limits<std::uint_fast32_t>::digits;
    std::subtract_with_carry_engine<std::uint_fast32_t, m, j, k> correct_generator;
    LaggedFibonacciGenerator<std::uint_fast32_t, m, j, k> our_generator;
    std::uint_fast32_t count_number = 15;
    std::vector<std::uint_fast32_t> correct_number;
    std::vector<std::uint_fast32_t> our_number;
    correct_number.reserve(count_number);
    our_number.reserve(count_number);
    for (std::uint_fast32_t i = 0; i < count_number; ++i) {
        correct_number.push_back(correct_generator());
        our_number.push_back(our_generator());
    }
    ASSERT_EQ(correct_number, our_number);
}

TEST(LFG, can_generator_correct_seq_7) {
    constexpr std::uint_fast32_t j = 7U;
    constexpr std::uint_fast32_t k = 10U;
    constexpr std::uint_fast32_t m = std::numeric_limits<std::uint_fast32_t>::digits;
    std::subtract_with_carry_engine<std::uint_fast32_t, m, j, k> correct_generator;
    LaggedFibonacciGenerator<std::uint_fast32_t, m, j, k> our_generator;
    std::uint_fast32_t count_number = 200;
    std::vector<std::uint_fast32_t> correct_number;
    std::vector<std::uint_fast32_t> our_number;
    correct_number.reserve(count_number);
    our_number.reserve(count_number);
    for (std::uint_fast32_t i = 0; i < count_number; ++i) {
        correct_number.push_back(correct_generator());
        our_number.push_back(our_generator());
    }
    ASSERT_EQ(correct_number, our_number);
}

// TEST(LFG, can_throw_exception_incorrect_type_int) {
//     constexpr std::int_fast64_t j = 10;
//     constexpr std::int_fast64_t k = 5;
//     constexpr std::int_fast64_t m = -1;
//     try {
//         LaggedFibonacciGenerator<std::int_fast64_t, j, k, m> correct_generator;
//     } catch (const BaseError &error) {
//         EXPECT_STREQ(error.what(), "LFG can work with unsigned integers");
//     }
// }

// TEST(LFG, can_throw_exception_incorrect_type_char) {
//     constexpr char32_t j = 10;
//     constexpr char32_t k = 5;
//     constexpr char32_t m = -1;
//     try {
//         LaggedFibonacciGenerator<char32_t, j, k, m> correct_generator;
//     } catch (const BaseError &error) {
//         EXPECT_STREQ(error.what(), "LFG can work with unsigned integers");
//     }
// }

// TEST(LFG, can_throw_exception_incorrect_type_uchart) {
//     constexpr std::uint_fast8_t j = 10U;
//     constexpr std::uint_fast8_t k = 10U;
//     constexpr std::uint_fast8_t m = 128U;
//     try {
//         LaggedFibonacciGenerator<uint_fast8_t, j, k, m> correct_generator;
//     } catch (const BaseError &error) {
//         FAIL();
//     }
//     SUCCEED();
// }

// TEST(LFG, can_throw_exception_incorrect_type_ushort) {
//     constexpr std::uint_fast16_t j = 10U;
//     constexpr std::uint_fast16_t k = 10U;
//     constexpr std::uint_fast16_t m = std::numeric_limits<std::uint_fast16_t>::digits;
//     try {
//         LaggedFibonacciGenerator<uint_fast16_t, j, k, m> correct_generator;
//     } catch (const BaseError &error) {
//         FAIL();
//     }
//     SUCCEED();
// }

// TEST(LFG, can_throw_exception_incorrect_type_ulong) {
//     constexpr std::uint_fast32_t j = 10U;
//     constexpr std::uint_fast32_t k = 10U;
//     constexpr std::uint_fast32_t m = std::numeric_limits<std::uint_fast32_t>::digits;
//     try {
//         LaggedFibonacciGenerator<uint_fast32_t, j, k, m> correct_generator;
//     } catch (const BaseError &error) {
//         FAIL();
//     }
//     SUCCEED();
// }

// TEST(LFG, can_throw_exception_incorrect_type_ulonglong) {
//     constexpr std::uint_fast64_t j = 10U;
//     constexpr std::uint_fast64_t k = 10U;
//     constexpr std::uint_fast64_t m = std::numeric_limits<std::uint_fast64_t>::digits;
//     try {
//         LaggedFibonacciGenerator<uint_fast64_t, j, k, m> correct_generator;
//     } catch (const BaseError &error) {
//         FAIL();
//     }
//     SUCCEED();
// }

// TEST(LFG, can_throw_exception_incorrect_type_6) {
//     constexpr std::uint_fast16_t j = 10U;
//     constexpr std::uint_fast16_t k = 10U;
//     constexpr std::uint_fast16_t m = std::numeric_limits<std::uint_fast16_t>::digits;
//     try {
//         LaggedFibonacciGenerator<uint_fast16_t, j, k, m> correct_generator;
//     } catch (const BaseError &error) {
//         FAIL();
//     }
//     SUCCEED();
// }

// TEST(LFG, can_throw_exception_modulus) {
//     constexpr std::uint_fast64_t j = 10;
//     constexpr std::uint_fast64_t k = 5;
//     constexpr std::uint_fast64_t m = -1;
//     try {
//         LaggedFibonacciGenerator<std::uint_fast64_t, j, k, m> correct_generator;
//     } catch (const BaseError &error) {
//         EXPECT_STREQ(error.what(), "Incorrect modulus");
//     }
// }

// TEST(LFG, can_throw_exception_multiplier_more_modulus) {
//     constexpr std::uint_fast64_t j = 2 << 16 + 1;
//     constexpr std::uint_fast64_t k = 5;
//     constexpr std::uint_fast64_t m = 2 << 16 - 1;
//     try {
//         LaggedFibonacciGenerator<std::uint_fast64_t, j, k, m> correct_generator;
//     } catch (const BaseError &error) {
//         EXPECT_STREQ(error.what(), "Incorrect multiplier");
//     }
// }

// TEST(LFG, can_throw_exception_increment_more_modulus) {
//     constexpr std::uint_fast64_t j = 10;
//     constexpr std::uint_fast64_t k = 2 << 16 + 1;
//     constexpr std::uint_fast64_t m = 2 << 16 - 1;
//     try {
//         LaggedFibonacciGenerator<std::uint_fast64_t, j, k, m> correct_generator;
//     } catch (const BaseError &error) {
//         EXPECT_STREQ(error.what(), "Incorrect increment");
//     }
// }

// TEST(LFG, can_throw_exception_seed_more_modules) {
//     constexpr std::uint_fast64_t j = 10;
//     constexpr std::uint_fast64_t k = 10;
//     constexpr std::uint_fast64_t m = 2 << 16 - 1;
//     try {
//         LaggedFibonacciGenerator<std::uint_fast64_t, j, k, m> correct_generator(2 << 16 + 1);
//     } catch (const BaseError &error) {
//         EXPECT_STREQ(error.what(), "Incorrect seed");
//     }
// }

// TEST(LFG, can_get_correct_min_if_modulus_not_zero) {
//     constexpr std::uint_fast32_t j = 7U;
//     constexpr std::uint_fast32_t k = 10U;
//     constexpr std::uint_fast32_t m = std::numeric_limits<std::uint_fast32_t>::digits;
//     std::subtract_with_carry_engine<std::uint_fast32_t, m, j, k> correct_generator;
//     LaggedFibonacciGenerator<std::uint_fast32_t, m, j, k> our_generator;
//     ASSERT_EQ(correct_generator.min(), our_generator.min());
// }

// TEST(LFG, can_get_correct_min_if_modulus_zero) {
//     constexpr std::uint_fast32_t j = 7U;
//     constexpr std::uint_fast32_t k = 10U;
//     constexpr std::uint_fast32_t m = std::numeric_limits<std::uint_fast32_t>::digits;
//     std::subtract_with_carry_engine<std::uint_fast32_t, m, j, k> correct_generator;
//     LaggedFibonacciGenerator<std::uint_fast32_t, m, j, k> our_generator;
//     ASSERT_EQ(correct_generator.min(), our_generator.min());
// }

// TEST(LFG, correct_work_discard_1) {
//     constexpr std::uint_fast32_t j = 7U;
//     constexpr std::uint_fast32_t k = 10U;
//     constexpr std::uint_fast32_t m = std::numeric_limits<std::uint_fast32_t>::digits;
//     constexpr std::uint_fast32_t z = 5U;
//     std::subtract_with_carry_engine<std::uint_fast32_t, m, j, k> correct_generator;
//     LaggedFibonacciGenerator<std::uint_fast32_t, m, j, k> our_generator;
//     correct_generator.discard(z);
//     our_generator.discard(z);
//     ASSERT_EQ(correct_generator(), our_generator());
// }

// TEST(LFG, correct_work_discard_2) {
//     constexpr std::uint_fast32_t j = 7U;
//     constexpr std::uint_fast32_t k = 10U;
//     constexpr std::uint_fast32_t m = std::numeric_limits<std::uint_fast32_t>::digits;
//     constexpr std::uint_fast32_t z = 0U;
//     std::subtract_with_carry_engine<std::uint_fast32_t, m, j, k> correct_generator;
//     LaggedFibonacciGenerator<std::uint_fast32_t, m, j, k> our_generator;
//     correct_generator.discard(z);
//     our_generator.discard(z);
//     ASSERT_EQ(correct_generator(), our_generator());
// }

// TEST(LFG, correct_work_discard_3) {
//     constexpr std::uint_fast32_t j = 7U;
//     constexpr std::uint_fast32_t k = 10U;
//     constexpr std::uint_fast32_t m = std::numeric_limits<std::uint_fast32_t>::digits;
//     constexpr std::uint_fast32_t z = 10U;
//     std::subtract_with_carry_engine<std::uint_fast32_t, m, j, k> correct_generator;
//     LaggedFibonacciGenerator<std::uint_fast32_t, m, j, k> our_generator;
//     correct_generator.discard(z);
//     our_generator.discard(z);
//     ASSERT_EQ(correct_generator(), our_generator());
// }

// TEST(LFG, correct_work_set_seed_1) {
//     constexpr std::uint_fast32_t j = 7U;
//     constexpr std::uint_fast32_t k = 10U;
//     constexpr std::uint_fast32_t m = std::numeric_limits<std::uint_fast32_t>::digits;
//     constexpr std::uint_fast32_t seed = 100U;
//     std::subtract_with_carry_engine<std::uint_fast32_t, m, j, k> correct_generator;
//     LaggedFibonacciGenerator<std::uint_fast32_t, m, j, k> our_generator;
//     correct_generator.seed(seed);
//     our_generator.seed(seed);
//     ASSERT_EQ(correct_generator(), our_generator());
// }

// TEST(LFG, correct_work_set_seed_2) {
//     constexpr std::uint_fast32_t j = 7U;
//     constexpr std::uint_fast32_t k = 10U;
//     constexpr std::uint_fast32_t m = std::numeric_limits<std::uint_fast32_t>::digits;
//     constexpr std::uint_fast32_t seed = 54321U;
//     std::subtract_with_carry_engine<std::uint_fast32_t, m, j, k> correct_generator;
//     LaggedFibonacciGenerator<std::uint_fast32_t, m, j, k> our_generator;
//     correct_generator.seed(seed);
//     our_generator.seed(seed);
//     ASSERT_EQ(correct_generator(), our_generator());
// }