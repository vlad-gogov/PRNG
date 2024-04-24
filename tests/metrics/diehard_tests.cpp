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
    double answer = 0.856633;
    ASSERT_NEAR(p, answer, abs_error);
}


TEST(Diehard, matrix_test) {
    utils::seq_bytes bytes = {0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1};

    int degrees_of_freedom = -1;
    double p = diehard::matrix_test(bytes, 3, 3, 2, degrees_of_freedom);
    double answer = 0.741948;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(Diehard, binary_matrix_rank_e) {
    utils::seq_bytes bytes = utils::read_bytes_from_file("tests/metrics/e.txt", 100'000);
    size_t M = 32;
    size_t Q = 32;
    int degrees_of_freedom = -1;
    double p = diehard::matrix_test(bytes, M, Q, 100'000 / (32 * 32), degrees_of_freedom);
    double answer = 0.306155;
    ASSERT_NEAR(p, answer, abs_error);
}
