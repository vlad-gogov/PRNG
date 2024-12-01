#include <gtest/gtest.h>

#include <generators/base_error.hpp>
#include <generators/mersenne_twister.hpp>
#include <metrics/nist_tests.hpp>

#include <iostream>
#include <random>

TEST(MT, can_generator) {
    MT19937 generator;
    std::cout << generator() << std::endl;
    ASSERT_EQ(1, 1);
}

TEST(MT, frequency_lcg) {
    MT19937 generator;
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

TEST(MT, frequency_block_lcg) {
    MT19937 generator;
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

TEST(MT, runs_lcg) {
    MT19937 generator;
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

TEST(MT, longest_run_of_ones_lcg) {
    MT19937 generator;
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

TEST(MT, binary_matrix_rank_lcg) {
    MT19937 generator;
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

TEST(MT, discrete_fourier_transform_lcg) {
    MT19937 generator;
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

TEST(MT, non_overlapping_template_matching_lcg) {
    MT19937 generator;

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

TEST(MT, overlapping_template_matching_lcg) {
    MT19937 generator;
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

TEST(MT, universal_digit_lcg) {
    MT19937 generator;
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

TEST(MT, serial_lcg) {
    MT19937 generator;
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

TEST(MT, approximate_entropy_lcg) {
    MT19937 generator;
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

TEST(MT, cumulative_sums_lcg_forward) {
    MT19937 generator;
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

TEST(MT, cumulative_sums_lcg_reverse) {
    MT19937 generator;
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

TEST(MT, random_excursions_lcg) {
    MT19937 generator;
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

TEST(MT, random_excursions_variant_lcg) {
    MT19937 generator;
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