#include <gtest/gtest.h>

#include "metrics/diehard_tests.hpp"

#include <iostream>

constexpr double abs_error = 1e-6;

TEST(Diehard, runs_test) {
    utils::seq_bytes bytes = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1,
                              0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1,
                              0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0,
                              0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0};
    double p = diehard::runs_test(bytes);
    double answer = 0.5007979;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Diehard, runs_test_e) {
    utils::seq_bytes bytes = utils::read_bits_from_exponent(100000);
    double p = diehard::runs_test(bytes);
    double answer = 0.4854955;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Diehard, matrix_test) {
    utils::seq_bytes bytes = {0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1};

    double p = diehard::matrix_test(bytes, 3, 3, 2);
    double answer = 0.8898351;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Diehard, binary_matrix_rank_e) {
    utils::seq_bytes bytes = utils::read_bits_from_exponent(100000);
    size_t M = 32;
    size_t Q = 32;
    double p = diehard::matrix_test(bytes, M, Q, 100000 / (32 * 32));
    double answer = 0.788588;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Diehard, birthdays_test_random) {
    int days_bits = 24;
    int num_bdays = 512;
    int tsamples = 100;
    utils::seq_bytes bytes = utils::read_bits_from_exponent(days_bits * num_bdays + tsamples * 32);
    double p = diehard::birthdays_test(bytes, days_bits, num_bdays, tsamples);
    double answer = 0.017107409200810778;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Diehard, minimum_distance_test_2d_random) {
    int n_dims = 2;
    int num_coordinates = 800;
    int num_samples = 10;
    utils::seq_bytes bytes = utils::read_bits_from_exponent(n_dims * num_coordinates * num_samples * 64);
    double p = diehard::minimum_distance_test(bytes, n_dims, num_coordinates, num_samples);
    double limit = 0.05;
    ASSERT_LE(p, limit);
}

TEST(Diehard, overlapping_permutations_test_random) {
    int num_samples = 1000;
    utils::seq_bytes bytes = utils::read_bits_from_exponent((num_samples + 4) * 32);
    double p = diehard::overlapping_permutations_test(bytes, num_samples);
    double answer = 0.477356439;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Diehard, minimum_distance_test_3d_random) {
    int n_dims = 3;
    int num_coordinates = 400;
    int num_samples = 10;
    utils::seq_bytes bytes = utils::read_bits_from_exponent(n_dims * num_coordinates * num_samples * 64);
    double p = diehard::minimum_distance_test(bytes, n_dims, num_coordinates, num_samples);
    double limit = 0.05;
    ASSERT_LE(p, limit);
}

TEST(Diehard, craps_test_random) {
    int num_games = 2000;
    // Need 2 rolls minimum per game, so allocate enough bytes
    utils::seq_bytes bytes = utils::read_bits_from_exponent(num_games * 4 * 32);
    double p = diehard::craps_test(bytes, num_games);
    // For truly random data, p-value should be > 0.01
    ASSERT_GT(p, 0.01);
}
