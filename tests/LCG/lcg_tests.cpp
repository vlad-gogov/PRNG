#include <gtest/gtest.h>

#include "generators/base_error.hpp"
#include "generators/linear_congruential_generator.hpp"
#include "metrics/nist_tests.hpp"
#include "metrics/utils.hpp"

#include <iostream>
#include <limits>
#include <random>

TEST(Lcg, can_generate_correct_seq_1) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    std::linear_congruential_engine<std::uint32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint32_t, a, c, m> my_generator;
    ASSERT_EQ(correct_generator(), my_generator());
}

TEST(Lcg, can_generate_correct_seq_2) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    std::linear_congruential_engine<std::uint32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint32_t, a, c, m> my_generator;
    std::uint32_t count_number = 5;
    std::vector<std::uint32_t> correct_number;
    std::vector<std::uint32_t> our_number;
    correct_number.reserve(count_number);
    our_number.reserve(count_number);
    for (std::uint32_t i = 0; i < count_number; ++i) {
        correct_number.push_back(correct_generator());
        our_number.push_back(my_generator());
    }
    ASSERT_EQ(correct_number, our_number);
}

TEST(Lcg, can_generate_correct_seq_3) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    std::linear_congruential_engine<std::uint32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint32_t, a, c, m> my_generator;
    std::uint32_t count_number = 10;
    std::vector<std::uint32_t> correct_number;
    std::vector<std::uint32_t> our_number;
    correct_number.reserve(count_number);
    our_number.reserve(count_number);
    for (std::uint32_t i = 0; i < count_number; ++i) {
        correct_number.push_back(correct_generator());
        our_number.push_back(my_generator());
    }
    ASSERT_EQ(correct_number, our_number);
}

TEST(Lcg, can_generate_correct_seq_4) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 5U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    std::linear_congruential_engine<std::uint32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint32_t, a, c, m> my_generator;
    ASSERT_EQ(correct_generator(), my_generator());
}

TEST(Lcg, can_generate_correct_seq_5) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 5U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    std::linear_congruential_engine<std::uint32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint32_t, a, c, m> my_generator;
    std::uint32_t count_number = 5;
    std::vector<std::uint32_t> correct_number;
    std::vector<std::uint32_t> our_number;
    correct_number.reserve(count_number);
    our_number.reserve(count_number);
    for (std::uint32_t i = 0; i < count_number; ++i) {
        correct_number.push_back(correct_generator());
        our_number.push_back(my_generator());
    }
    ASSERT_EQ(correct_number, our_number);
}

TEST(Lcg, can_generate_correct_seq_6) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 5U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    std::linear_congruential_engine<std::uint32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint32_t, a, c, m> my_generator;
    std::uint32_t count_number = 15;
    std::vector<std::uint32_t> correct_number;
    std::vector<std::uint32_t> our_number;
    correct_number.reserve(count_number);
    our_number.reserve(count_number);
    for (std::uint32_t i = 0; i < count_number; ++i) {
        correct_number.push_back(correct_generator());
        our_number.push_back(my_generator());
    }
    ASSERT_EQ(correct_number, our_number);
}

TEST(Lcg, can_generate_correct_seq_7) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 5U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    std::linear_congruential_engine<std::uint32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint32_t, a, c, m> my_generator;
    std::uint32_t count_number = 200;
    std::vector<std::uint32_t> correct_number;
    std::vector<std::uint32_t> our_number;
    correct_number.reserve(count_number);
    our_number.reserve(count_number);
    for (std::uint32_t i = 0; i < count_number; ++i) {
        correct_number.push_back(correct_generator());
        our_number.push_back(my_generator());
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
    LinearCongruentialGenerator<std::uint32_t, a, c, m> my_generator;
    ASSERT_EQ(correct_generator.min(), my_generator.min());
}

TEST(Lcg, can_get_correct_min_if_modulus_zero) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    std::linear_congruential_engine<std::uint32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint32_t, a, c, m> my_generator;
    ASSERT_EQ(correct_generator.min(), my_generator.min());
}

TEST(Lcg, correct_work_discard_1) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    constexpr std::uint32_t z = 5U;
    std::linear_congruential_engine<std::uint32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint32_t, a, c, m> my_generator;
    correct_generator.discard(z);
    my_generator.discard(z);
    ASSERT_EQ(correct_generator(), my_generator());
}

TEST(Lcg, correct_work_discard_2) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    constexpr std::uint32_t z = 0U;
    std::linear_congruential_engine<std::uint32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint32_t, a, c, m> my_generator;
    correct_generator.discard(z);
    my_generator.discard(z);
    ASSERT_EQ(correct_generator(), my_generator());
}

TEST(Lcg, correct_work_discard_3) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    constexpr std::uint32_t z = 10U;
    std::linear_congruential_engine<std::uint32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint32_t, a, c, m> my_generator;
    correct_generator.discard(z);
    my_generator.discard(z);
    ASSERT_EQ(correct_generator(), my_generator());
}

TEST(Lcg, correct_work_set_seed_1) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    constexpr std::uint32_t seed = 50U;
    std::linear_congruential_engine<std::uint32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint32_t, a, c, m> my_generator;
    correct_generator.seed(seed);
    my_generator.seed(seed);
    ASSERT_EQ(correct_generator(), my_generator());
}

TEST(Lcg, correct_work_set_seed_2) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    constexpr std::uint32_t seed = 54321U;
    std::linear_congruential_engine<std::uint32_t, a, c, m> correct_generator;
    LinearCongruentialGenerator<std::uint32_t, a, c, m> my_generator;
    correct_generator.seed(seed);
    my_generator.seed(seed);
    ASSERT_EQ(correct_generator(), my_generator());
}

TEST(Lcg, frequency_lcg) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    LinearCongruentialGenerator<std::uint32_t, a, c, m> generator(23482349);
    std::uint32_t count_number = 50;
    std::vector<std::uint32_t> numbers(count_number);
    for (std::uint32_t i = 0; i < count_number; ++i) {
        numbers[i] = generator();
    }
    utils::seq_bytes bytes = utils::convert_numbers_to_seq_bytes(numbers);
    double p = nist::frequency_test(bytes);
    std::cout << "P-value: " << p << std::endl;
    ASSERT_TRUE(nist::check_frequency_test(bytes));
}

TEST(Lcg, frequency_block_lcg) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    LinearCongruentialGenerator<std::uint32_t, a, c, m> generator(23482349);
    std::uint32_t count_number = 50;
    std::vector<std::uint32_t> numbers(count_number);
    for (std::uint32_t i = 0; i < count_number; ++i) {
        numbers[i] = generator();
    }
    utils::seq_bytes bytes = utils::convert_numbers_to_seq_bytes(numbers);
    double p = nist::frequency_block_test(bytes, 128);
    std::cout << "P-value: " << p << std::endl;
    ASSERT_TRUE(nist::check_frequency_block_test(bytes, 128));
}

TEST(Lcg, runs_lcg) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    LinearCongruentialGenerator<std::uint32_t, a, c, m> generator(23482349);
    std::uint32_t count_number = 50;
    std::vector<std::uint32_t> numbers(count_number);
    for (std::uint32_t i = 0; i < count_number; ++i) {
        numbers[i] = generator();
    }
    utils::seq_bytes bytes = utils::convert_numbers_to_seq_bytes(numbers);
    double p = nist::runs_test(bytes);
    std::cout << "P-value: " << p << std::endl;
    ASSERT_TRUE(nist::check_runs_test(bytes));
}

TEST(Lcg, longest_run_of_ones_lcg) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    LinearCongruentialGenerator<std::uint32_t, a, c, m> generator(23482349);
    std::uint32_t count_number = 50;
    std::vector<std::uint32_t> numbers(count_number);
    for (std::uint32_t i = 0; i < count_number; ++i) {
        numbers[i] = generator();
    }
    utils::seq_bytes bytes = utils::convert_numbers_to_seq_bytes(numbers);
    double p = nist::longest_run_of_ones(bytes);
    std::cout << "P-value: " << p << std::endl;
    ASSERT_TRUE(nist::check_longest_run_of_ones(bytes));
}

TEST(Lcg, binary_matrix_rank_lcg) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    LinearCongruentialGenerator<std::uint32_t, a, c, m> generator(23482349);
    std::uint32_t count_number = 50;
    std::vector<std::uint32_t> numbers(count_number);
    for (std::uint32_t i = 0; i < count_number; ++i) {
        numbers[i] = generator();
    }
    utils::seq_bytes bytes = utils::convert_numbers_to_seq_bytes(numbers);
    size_t M = 32;
    size_t Q = 32;
    double p = nist::binary_matrix_rank(bytes, M, Q);
    std::cout << "P-value: " << p << std::endl;
    ASSERT_TRUE(nist::check_binary_matrix_rank(bytes, M, Q));
}

TEST(Lcg, discrete_fourier_transform_lcg) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    LinearCongruentialGenerator<std::uint32_t, a, c, m> generator(23482349);
    std::uint32_t count_number = 50;
    std::vector<std::uint32_t> numbers(count_number);
    for (std::uint32_t i = 0; i < count_number; ++i) {
        numbers[i] = generator();
    }
    utils::seq_bytes bytes = utils::convert_numbers_to_seq_bytes(numbers);
    double p = nist::discrete_fourier_transform(bytes);
    std::cout << "P-value: " << p << std::endl;
    ASSERT_TRUE(nist::check_discrete_fourier_transform(bytes));
}

TEST(Lcg, non_overlapping_template_matching_lcg) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    LinearCongruentialGenerator<std::uint32_t, a, c, m> generator(23482349);

    std::uint32_t count_number = 50;
    std::vector<std::uint32_t> numbers(count_number);
    for (std::uint32_t i = 0; i < count_number; ++i) {
        numbers[i] = generator();
    }
    utils::seq_bytes bytes = utils::convert_numbers_to_seq_bytes(numbers);
    utils::seq_bytes template_ = {0, 0, 0, 0, 0, 0, 0, 0, 1};
    double p = nist::non_overlapping_template_matching(bytes, template_);
    std::cout << "P-value: " << p << std::endl;
    ASSERT_TRUE(nist::check_non_overlapping_template_matching(bytes, template_));
}

TEST(Lcg, overlapping_template_matching_lcg) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    LinearCongruentialGenerator<std::uint32_t, a, c, m> generator(23482349);
    std::uint32_t count_number = 50;
    std::vector<std::uint32_t> numbers(count_number);
    for (std::uint32_t i = 0; i < count_number; ++i) {
        numbers[i] = generator();
    }
    utils::seq_bytes bytes = utils::convert_numbers_to_seq_bytes(numbers);
    utils::seq_bytes template_ = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    double p = nist::overlapping_template_matching(bytes, template_);
    std::cout << "P-value: " << p << std::endl;
    ASSERT_TRUE(nist::check_overlapping_template_matching(bytes, template_));
}

TEST(Lcg, universal_digit_lcg) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    LinearCongruentialGenerator<std::uint32_t, a, c, m> generator(23482349);
    std::uint32_t count_number = 50;
    std::vector<std::uint32_t> numbers(count_number);
    for (std::uint32_t i = 0; i < count_number; ++i) {
        numbers[i] = generator();
    }
    utils::seq_bytes bytes = utils::convert_numbers_to_seq_bytes(numbers);
    double p = nist::universal(bytes);
    std::cout << "P-value: " << p << std::endl;
    ASSERT_TRUE(nist::check_universal(bytes));
}

TEST(Lcg, serial_lcg) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    LinearCongruentialGenerator<std::uint32_t, a, c, m> generator(23482349);
    std::uint32_t count_number = 50;
    std::vector<std::uint32_t> numbers(count_number);
    for (std::uint32_t i = 0; i < count_number; ++i) {
        numbers[i] = generator();
    }
    utils::seq_bytes bytes = utils::convert_numbers_to_seq_bytes(numbers);
    auto [p1, p2] = nist::serial_complexity(bytes, 16);
    std::cout << "P-value: " << p1 << " " << p2 << std::endl;
    ASSERT_TRUE(nist::check_serial_complexity(bytes, 16));
}

TEST(Lcg, approximate_entropy_lcg) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    LinearCongruentialGenerator<std::uint32_t, a, c, m> generator(23482349);
    std::uint32_t count_number = 50;
    std::vector<std::uint32_t> numbers(count_number);
    for (std::uint32_t i = 0; i < count_number; ++i) {
        numbers[i] = generator();
    }
    utils::seq_bytes bytes = utils::convert_numbers_to_seq_bytes(numbers);
    double p = nist::approximate_entropy(bytes, 10);
    std::cout << "P-value: " << p << std::endl;
    ASSERT_TRUE(nist::check_approximate_entropy(bytes, 10));
}

TEST(Lcg, cumulative_sums_lcg_forward) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    LinearCongruentialGenerator<std::uint32_t, a, c, m> generator(23482349);
    std::uint32_t count_number = 50;
    std::vector<std::uint32_t> numbers(count_number);
    for (std::uint32_t i = 0; i < count_number; ++i) {
        numbers[i] = generator();
    }
    utils::seq_bytes bytes = utils::convert_numbers_to_seq_bytes(numbers);
    double p = nist::cumulative_sums(bytes, nist::CumulativeSumsMode::Forward);
    std::cout << "P-value: " << p << std::endl;
    ASSERT_TRUE(nist::check_cumulative_sums(bytes, nist::CumulativeSumsMode::Forward));
}

TEST(Lcg, cumulative_sums_lcg_reverse) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    LinearCongruentialGenerator<std::uint32_t, a, c, m> generator(23482349);
    std::uint32_t count_number = 50;
    std::vector<std::uint32_t> numbers(count_number);
    for (std::uint32_t i = 0; i < count_number; ++i) {
        numbers[i] = generator();
    }
    utils::seq_bytes bytes = utils::convert_numbers_to_seq_bytes(numbers);
    double p = nist::cumulative_sums(bytes, nist::CumulativeSumsMode::Reverse);
    std::cout << "P-value: " << p << std::endl;
    ASSERT_TRUE(nist::check_cumulative_sums(bytes, nist::CumulativeSumsMode::Reverse));
}

TEST(Lcg, random_excursions_lcg) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    LinearCongruentialGenerator<std::uint32_t, a, c, m> generator(23482349);
    std::uint32_t count_number = 50;
    std::vector<std::uint32_t> numbers(count_number);
    for (std::uint32_t i = 0; i < count_number; ++i) {
        numbers[i] = generator();
    }
    utils::seq_bytes bytes = utils::convert_numbers_to_seq_bytes(numbers);
    std::vector<double> p_values = nist::random_excursions(bytes);
    std::cout << "P-values: ";
    for (size_t i = 0; i < p_values.size(); i++) {
        std::cout << p_values[i] << " ";
    }
    std::cout << std::endl;
    std::vector<bool> answers = nist::check_random_excursions(bytes);
    for (const auto &answer : answers) {
        ASSERT_TRUE(answer);
    }
}

TEST(Lcg, random_excursions_variant_lcg) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    LinearCongruentialGenerator<std::uint32_t, a, c, m> generator(23482349);
    std::uint32_t count_number = 50;
    std::vector<std::uint32_t> numbers(count_number);
    for (std::uint32_t i = 0; i < count_number; ++i) {
        numbers[i] = generator();
    }
    utils::seq_bytes bytes = utils::convert_numbers_to_seq_bytes(numbers);
    std::vector<double> p_values = nist::random_excursions_variant(bytes);
    std::cout << "P-values: ";
    for (size_t i = 0; i < p_values.size(); i++) {
        std::cout << p_values[i] << " ";
    }
    std::cout << std::endl;
    std::vector<bool> answers = nist::check_random_excursions_variant(bytes);
    for (const auto &answer : answers) {
        ASSERT_TRUE(answer);
    }
}
