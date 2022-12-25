#include <gtest/gtest.h>

#include "metrics/nist_tests.hpp"

#include <iostream>

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
