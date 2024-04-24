#include <gtest/gtest.h>

#include <random>

#include "metrics/nist_tests.hpp"

constexpr double abs_error = 1e-6;

TEST(Nist, frequency_1) {
    utils::seq_bytes bytes = {1, 0, 1, 1, 0, 1, 0, 1, 0, 1};
    double p = nist::frequency_test(bytes);
    double answer = 0.527089;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, frequency_2) {
    utils::seq_bytes bytes = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1,
                              0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1,
                              0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0,
                              0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0};
    double p = nist::frequency_test(bytes);
    double answer = 0.109599;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, frequency_block_1) {
    utils::seq_bytes bytes = {0, 1, 1, 0, 0, 1, 1, 0, 1, 0};
    size_t block_size = 3U;
    double p = nist::frequency_block_test(bytes, block_size);
    double answer = 0.801252;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, frequency_block_2) {
    utils::seq_bytes bytes = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1,
                              0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1,
                              0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0,
                              0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0};
    size_t block_size = 10U;
    double p = nist::frequency_block_test(bytes, block_size);
    double answer = 0.706438;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, runs_1) {
    utils::seq_bytes bytes = {1, 0, 0, 1, 1, 0, 1, 0, 1, 1};
    double p = nist::runs_test(bytes);
    double answer = 0.147232;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, runs_2) {
    utils::seq_bytes bytes = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1,
                              0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1,
                              0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0,
                              0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0};
    double p = nist::runs_test(bytes);
    double answer = 0.500798;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, longest_run_of_ones_1) {
    utils::seq_bytes bytes = {1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1,
                              0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0,
                              1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1,
                              1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0,
                              1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0};
    double p = nist::longest_run_of_ones(bytes);
    double answer = 0.180598;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, binary_matrix_rank_1) {
    utils::seq_bytes bytes = {0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1};
    size_t M = 3;
    size_t Q = 3;
    double p = nist::binary_matrix_rank(bytes, M, Q);
    double answer = 0.741948;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, binary_matrix_rank_e) {
    utils::seq_bytes bytes = utils::read_bytes_from_file("tests/metrics/e.txt", 100'000);
    size_t M = 32;
    size_t Q = 32;
    double p = nist::binary_matrix_rank(bytes, M, Q);
    double answer = 0.306155;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, binary_matrix_rank_random) {
    std::random_device rd;
    std::mt19937 rng{rd()};
    std::uniform_int_distribution<unsigned int> uid(0, (1 << 31) - 1);
    size_t count_numbers = 100'000;
    std::vector<unsigned int> numbers(count_numbers);
    for (size_t i = 0; i < count_numbers; i++) {
        numbers[i] = uid(rng);
    }
    utils::seq_bytes bytes = utils::convert_numbers_to_seq_bytes(numbers);
    size_t M = 5;
    size_t Q = 5;
    ASSERT_TRUE(nist::check_binary_matrix_rank(bytes, M, Q));
    // double p = nist::binary_matrix_rank(bytes, M, Q);
    // double answer = 0.306155;
    // ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, discrete_fourier_transform_1) {
    utils::seq_bytes bytes = {1, 0, 0, 1, 0, 1, 0, 0, 1, 1};
    double p = nist::discrete_fourier_transform(bytes);
    double answer = 0.029523;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, discrete_fourier_transform_2) {
    utils::seq_bytes bytes = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1,
                              0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1,
                              0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0,
                              0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0};
    double p = nist::discrete_fourier_transform(bytes);
    double answer = 0.168669;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, non_overlapping_template_matching_1) {
    utils::seq_bytes bytes = {1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0};
    utils::seq_bytes template_ = {0, 0, 1};
    double p = nist::non_overlapping_template_matching(bytes, template_, 2);
    double answer = 0.344154;
    ASSERT_NEAR(p, answer, abs_error);
}

// TEST(Nist, non_overlapping_template_matching_2) {
//     utils::seq_bytes bytes = utils::read_bytes_from_file("tests/metrics/sha1.txt", 1 << 20);
//     utils::seq_bytes template_ = {0, 0, 0, 0, 0, 0, 0, 0, 1};
//     double p = nist::non_overlapping_template_matching(bytes, template_);
//     double answer = 0.344154;
//     ASSERT_NEAR(p, answer, abs_error);
// }

TEST(Nist, overlapping_template_matching_1) {
    utils::seq_bytes bytes = {1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0,
                              0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1};
    utils::seq_bytes template_ = {1, 1};
    double p = nist::overlapping_template_matching(bytes, template_, 5);
    double answer = 0.409632;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, overlapping_template_matching_2) {
    utils::seq_bytes bytes = utils::read_bytes_from_file("tests/metrics/e.txt", 1'000'000);
    utils::seq_bytes template_ = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    double p = nist::overlapping_template_matching(bytes, template_, 5);
    double answer = 0.110434;
    ASSERT_NEAR(p, answer, abs_error);
}