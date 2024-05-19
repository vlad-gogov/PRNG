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
    utils::seq_bytes bytes = utils::read_bytes_from_file("tests/metrics/e.txt", 100000);
    double p = diehard::runs_test(bytes);
    double answer = 0.4854955;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Diehard, matrix_test) {
    utils::seq_bytes bytes = {0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1};

    double p = diehard::matrix_test(bytes, 3, 3, 2);
    double answer = 0.0645713;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Diehard, binary_matrix_rank_e) {
    utils::seq_bytes bytes = utils::read_bytes_from_file("tests/metrics/e.txt", 100000);
    size_t M = 32;
    size_t Q = 32;
    double p = diehard::matrix_test(bytes, M, Q, 100000 / (32 * 32));
    double answer = 0.171961;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Diehard, birthdays_test_random) {
    int days_bits = 24;
    int num_bdays = 512;
    int tsamples = 100;
    utils::seq_bytes bytes =
        utils::read_bytes_from_file("tests/metrics/random.txt", days_bits * num_bdays + tsamples * 32);
    double p = diehard::birthdays_test(bytes, days_bits, num_bdays, tsamples);
    double answer = 0.1195526;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Diehard, minimum_distance_test_2d_random) {
    int n_dims = 2;
    int num_coordinates = 800;
    int num_samples = 10;
    utils::seq_bytes bytes =
        utils::read_bytes_from_file("tests/metrics/random.txt", n_dims * num_coordinates * num_samples * 64);
    double p = diehard::minimum_distance_test(bytes, n_dims, num_coordinates, num_samples);
    double limit = 0.05;
    ASSERT_LE(p, limit);
}

TEST(Diehard, overlapping_permutations_test_random) {
    int num_samples = 1000;
    utils::seq_bytes bytes = utils::read_bytes_from_file("tests/metrics/random.txt", (num_samples + 4) * 32);
    double p = diehard::overlapping_permutations_test(bytes, num_samples);
    double answer = 0.5734710;
    ASSERT_NEAR(p, answer, abs_error);
}
