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
    double answer = 0.741908;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, binary_matrix_rank_e) {
    utils::seq_bytes bytes = utils::read_bits_from_exponent(100'000);
    size_t M = 32;
    size_t Q = 32;
    double p = nist::binary_matrix_rank(bytes, M, Q);
    double answer = 0.532069;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, discrete_fourier_transform_1) {
    utils::seq_bytes bytes = {1, 0, 0, 1, 0, 1, 0, 0, 1, 1};
    double p = nist::discrete_fourier_transform(bytes);
    double answer = 0.468160;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, discrete_fourier_transform_2) {
    utils::seq_bytes bytes = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1,
                              0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1,
                              0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0,
                              0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0};
    double p = nist::discrete_fourier_transform(bytes);
    double answer = 0.646355;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, non_overlapping_template_matching_1) {
    utils::seq_bytes bytes = {1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0};
    utils::seq_bytes template_ = {0, 0, 1};
    double p = nist::non_overlapping_template_matching(bytes, template_, 2);
    double answer = 0.344154;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, overlapping_template_matching_1) {
    utils::seq_bytes bytes = {1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0,
                              0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1};
    utils::seq_bytes template_ = {1, 1};
    double p = nist::overlapping_template_matching(bytes, template_, 10, 5, 5, true);
    double answer = 0.409632;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, overlapping_template_matching_2) {
    utils::seq_bytes bytes = utils::read_bits_from_exponent(1'000'000);
    utils::seq_bytes template_ = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    double p = nist::overlapping_template_matching(bytes, template_, 1032, 968, 5);
    double answer = 0.110434;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, linear_complexity) {
    utils::seq_bytes bytes = utils::read_bits_from_exponent(1'000'000);
    double p = nist::linear_complexity(bytes, 1000);
    double answer = 0.845406;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, serial_1) {
    utils::seq_bytes bytes = {0, 0, 1, 1, 0, 1, 1, 1, 0, 1};
    auto [p1, p2] = nist::serial_complexity(bytes, 3);
    double answer1 = 0.808792;
    double answer2 = 0.670320;
    ASSERT_NEAR(p1, answer1, abs_error);
    ASSERT_NEAR(p2, answer2, abs_error);
}

TEST(Nist, serial_2) {
    utils::seq_bytes bytes = utils::read_bits_from_exponent(1'000'000);
    auto [p1, p2] = nist::serial_complexity(bytes, 2);
    double answer1 = 0.843764;
    double answer2 = 0.561915;
    ASSERT_NEAR(p1, answer1, abs_error);
    ASSERT_NEAR(p2, answer2, abs_error);
}

TEST(Nist, approximate_entropy_1) {
    utils::seq_bytes bytes = {0, 1, 0, 0, 1, 1, 0, 1, 0, 1};
    double p = nist::approximate_entropy(bytes, 3);
    double answer = 0.261961;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, approximate_entropy_2) {
    utils::seq_bytes bytes = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1,
                              0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1,
                              0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0,
                              0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0};
    double p = nist::approximate_entropy(bytes, 2);
    double answer = 0.235301;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, cumulative_sums_1_forward) {
    utils::seq_bytes bytes = {1, 0, 1, 1, 0, 1, 0, 1, 1, 1};
    double p = nist::cumulative_sums(bytes, nist::CumulativeSumsMode::Forward);
    double answer = 0.4116588;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, cumulative_sums_1_reverse) {
    utils::seq_bytes bytes = {1, 0, 1, 1, 0, 1, 0, 1, 1, 1};
    double p = nist::cumulative_sums(bytes, nist::CumulativeSumsMode::Reverse);
    double answer = 0.4116588;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, cumulative_sums_2_forward) {
    utils::seq_bytes bytes = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1,
                              0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1,
                              0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0,
                              0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0};
    double p = nist::cumulative_sums(bytes, nist::CumulativeSumsMode::Forward);
    double answer = 0.219194;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, cumulative_sums_2_reverse) {
    utils::seq_bytes bytes = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1,
                              0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1,
                              0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0,
                              0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0};
    double p = nist::cumulative_sums(bytes, nist::CumulativeSumsMode::Reverse);
    double answer = 0.114866;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, random_excursions_1) {
    utils::seq_bytes bytes = {0, 1, 1, 0, 1, 1, 0, 1, 0, 1};
    std::vector<double> p_values = nist::random_excursions(bytes);
    std::vector<double> answers = {0.994506, 0.988003, 0.962566, 0.962566, 0.502488, 0.142514, 0.988003, 0.994506};
    for (size_t i = 0; i < answers.size(); i++) {
        ASSERT_NEAR(p_values[i], answers[i], abs_error);
    }
}

TEST(Nist, random_excursions_2) {
    utils::seq_bytes bytes = utils::read_bits_from_exponent(1'000'000);
    std::vector<double> p_values = nist::random_excursions(bytes);
    std::vector<double> answers = {0.573306, 0.197996, 0.164011, 0.007779, 0.786868, 0.440912, 0.797854, 0.778186};
    for (size_t i = 0; i < answers.size(); i++) {
        ASSERT_NEAR(p_values[i], answers[i], abs_error);
    }
}

TEST(Nist, random_excursions_variant_1) {
    utils::seq_bytes bytes = {0, 1, 1, 0, 1, 1, 0, 1, 0, 1};
    std::vector<double> p_values = nist::random_excursions_variant(bytes, false);
    std::vector<double> answers = {0.766433, 0.751830, 0.734095, 0.711923, 0.683091, 0.643429,
                                   0.583882, 0.479500, 0.414216, 0.683091, 1.000000, 0.583882,
                                   0.643429, 0.683091, 0.711923, 0.734095, 0.751830, 0.766433};
    for (size_t i = 0; i < answers.size(); i++) {
        ASSERT_NEAR(p_values[i], answers[i], abs_error);
    }
}

TEST(Nist, random_excursions_variant_2) {
    utils::seq_bytes bytes = utils::read_bits_from_exponent(1'000'000);
    std::vector<double> p_values = nist::random_excursions_variant(bytes, false);
    std::vector<double> answers = {0.858946, 0.794755, 0.576249, 0.493417, 0.633873, 0.917283,
                                   0.934708, 0.816012, 0.826009, 0.137861, 0.200642, 0.441254,
                                   0.939291, 0.505683, 0.445935, 0.512207, 0.538635, 0.593930};
    for (size_t i = 0; i < answers.size(); i++) {
        ASSERT_NEAR(p_values[i], answers[i], abs_error);
    }
}

TEST(Nist, frequency_digit_e) {
    utils::seq_bytes bytes = utils::read_bits_from_exponent();
    double p = nist::frequency_test(bytes);
    double answer = 0.926876;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, frequency_block_digit_e) {
    utils::seq_bytes bytes = utils::read_bits_from_exponent();
    double p = nist::frequency_block_test(bytes, 128);
    double answer = 0.200497;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, runs_digit_e) {
    utils::seq_bytes bytes = utils::read_bits_from_exponent();
    double p = nist::runs_test(bytes);
    double answer = 0.610927;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, longest_run_of_ones_digit_e) {
    utils::seq_bytes bytes = utils::read_bits_from_exponent();
    double p = nist::longest_run_of_ones(bytes);
    double answer = 0.718945;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, binary_matrix_rank_digit_e) {
    utils::seq_bytes bytes = utils::read_bits_from_exponent();
    size_t M = 32;
    size_t Q = 32;
    double p = nist::binary_matrix_rank(bytes, M, Q);
    double answer = 0.284126;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, discrete_fourier_transform_digit_e) {
    utils::seq_bytes bytes = utils::read_bits_from_exponent(1000);
    double p = nist::discrete_fourier_transform(bytes);
    double answer = 0.561658;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, non_overlapping_template_matching_digit_e) {
    utils::seq_bytes bytes = utils::read_bits_from_exponent();
    utils::seq_bytes template_ = {0, 0, 0, 0, 0, 0, 0, 0, 1};
    double p = nist::non_overlapping_template_matching(bytes, template_);
    double answer = 0.138094;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, overlapping_template_matching_digit_e) {
    utils::seq_bytes bytes = utils::read_bits_from_exponent();
    utils::seq_bytes template_ = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    double p = nist::overlapping_template_matching(bytes, template_, 1032, 968, 5);
    double answer = 0.110433;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, universal_digit_digit_e) {
    utils::seq_bytes bytes = utils::read_bits_from_exponent();
    double p = nist::universal(bytes);
    double answer = 0.273105;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, serial_digit_e) {
    utils::seq_bytes bytes = utils::read_bits_from_exponent();
    auto [p1, p2] = nist::serial_complexity(bytes, 16);
    double answer1 = 0.751935;
    double answer2 = 0.434688;
    ASSERT_NEAR(p1, answer1, abs_error);
    ASSERT_NEAR(p2, answer2, abs_error);
}

TEST(Nist, approximate_entropy_digit_e) {
    utils::seq_bytes bytes = utils::read_bits_from_exponent();
    double p = nist::approximate_entropy(bytes, 10);
    double answer = 0.679065;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, cumulative_sums_digit_e_forward) {
    utils::seq_bytes bytes = utils::read_bits_from_exponent();
    double p = nist::cumulative_sums(bytes, nist::CumulativeSumsMode::Forward);
    double answer = 0.672055;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, cumulative_sums_digit_e_reverse) {
    utils::seq_bytes bytes = utils::read_bits_from_exponent();
    double p = nist::cumulative_sums(bytes, nist::CumulativeSumsMode::Reverse);
    double answer = 0.758083;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Nist, random_excursions_digit_e) {
    utils::seq_bytes bytes = utils::read_bits_from_exponent();
    std::vector<double> p_values = nist::random_excursions(bytes);
    std::vector<double> answers = {
        0.573306, 0.197996, 0.164011, 0.007779, 0.786868, 0.440912, 0.797854, 0.778186,
    };
    for (size_t i = 0; i < answers.size(); i++) {
        ASSERT_NEAR(p_values[i], answers[i], abs_error);
    }
}

TEST(Nist, random_excursions_variant_digit_e) {
    utils::seq_bytes bytes = utils::read_bits_from_exponent();
    std::vector<double> p_values = nist::random_excursions_variant(bytes, false);
    std::vector<double> answers = {
        0.858946, 0.794755, 0.576249, 0.493417, 0.633873, 0.917283, 0.934708, 0.816012, 0.826009,
        0.137861, 0.200642, 0.441254, 0.939291, 0.505683, 0.445935, 0.512207, 0.538635, 0.593930,
    };
    for (size_t i = 0; i < answers.size(); i++) {
        ASSERT_NEAR(p_values[i], answers[i], abs_error);
    }
}
