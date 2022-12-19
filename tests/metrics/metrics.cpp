#include <gtest/gtest.h>

#include "metrics/nist_tests.hpp"
#include "metrics/utils.hpp"

#include <iostream>

constexpr double abs_error = 1e-6;

TEST(utils, can_convert_number_to_seq_bytes_1) {
    std::uint_fast32_t number = 100U;
    utils::seq_bytes seq = utils::convert_number_to_seq_bytes<std::uint_fast32_t>(number);
    utils::seq_bytes answer = {1, 1, 0, 0, 1, 0, 0};
    ASSERT_EQ(seq, answer);
}

TEST(utils, can_convert_number_to_seq_bytes_2) {
    std::uint_fast32_t number = 0U;
    utils::seq_bytes seq = utils::convert_number_to_seq_bytes<std::uint_fast32_t>(number);
    utils::seq_bytes answer = {0};
    ASSERT_EQ(seq, answer);
}

TEST(utils, can_convert_number_to_seq_bytes_3) {
    std::uint_fast32_t number = 1U;
    utils::seq_bytes seq = utils::convert_number_to_seq_bytes<std::uint_fast32_t>(number);
    utils::seq_bytes answer = {1};
    ASSERT_EQ(seq, answer);
}

TEST(utils, can_convert_number_to_seq_bytes_4) {
    std::uint_fast32_t number = 4U;
    utils::seq_bytes seq = utils::convert_number_to_seq_bytes<std::uint_fast32_t>(number);
    utils::seq_bytes answer = {1, 0, 0};
    ASSERT_EQ(seq, answer);
}

TEST(utils, can_convert_number_to_seq_bytes_5) {
    std::uint_fast32_t number = 16U;
    utils::seq_bytes seq = utils::convert_number_to_seq_bytes<std::uint_fast32_t>(number);
    utils::seq_bytes answer = {1, 0, 0, 0, 0};
    ASSERT_EQ(seq, answer);
}

TEST(utils, can_convert_number_to_seq_bytes_6) {
    std::uint_fast32_t number = 12345U;
    utils::seq_bytes seq = utils::convert_number_to_seq_bytes<std::uint_fast32_t>(number);
    utils::seq_bytes answer = {1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1};
    ASSERT_EQ(seq, answer);
}

TEST(utils, can_convert_numbers_to_seq_bytes_1) {
    std::vector<std::uint_fast32_t> numbers = {100U, 100U};
    utils::seq_bytes seq = utils::convert_numbers_to_seq_bytes<std::uint_fast32_t>(numbers);
    utils::seq_bytes answer = {1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0};
    ASSERT_EQ(seq, answer);
}

TEST(utils, can_convert_numbers_to_seq_bytes_2) {
    std::vector<std::uint_fast32_t> numbers = {0U, 1U};
    utils::seq_bytes seq = utils::convert_numbers_to_seq_bytes<std::uint_fast32_t>(numbers);
    utils::seq_bytes answer = {0, 1};
    ASSERT_EQ(seq, answer);
}

TEST(utils, can_convert_numbers_to_seq_bytes_3) {
    std::vector<std::uint_fast32_t> numbers = {1U, 4U, 16U, 12345U};
    utils::seq_bytes seq = utils::convert_numbers_to_seq_bytes<std::uint_fast32_t>(numbers);
    utils::seq_bytes answer = {1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1};
    ASSERT_EQ(seq, answer);
}

TEST(nist, frequency_1) {
    utils::seq_bytes bytes = {1, 0, 1, 1, 0, 1, 0, 1, 0, 1};
    double p = nist::frequency_test(bytes);
    double answer = 0.527089;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(nist, frequency_2) {
    utils::seq_bytes bytes = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1,
                              0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1,
                              0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0,
                              0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0};
    double p = nist::frequency_test(bytes);
    double answer = 0.109599;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(nist, frequency_block_1) {
    utils::seq_bytes bytes = {0, 1, 1, 0, 0, 1, 1, 0, 1, 0};
    size_t block_size = 3U;
    double p = nist::frequency_block_test(bytes, block_size);
    double answer = 0.801252;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(nist, frequency_block_2) {
    utils::seq_bytes bytes = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1,
                              0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1,
                              0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0,
                              0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0};
    size_t block_size = 10U;
    double p = nist::frequency_block_test(bytes, block_size);
    double answer = 0.706438;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(nist, runs_1) {
    utils::seq_bytes bytes = {1, 0, 0, 1, 1, 0, 1, 0, 1, 1};
    double p = nist::runs_test(bytes);
    double answer = 0.147232;
    ASSERT_NEAR(p, answer, abs_error);
}

TEST(nist, runs_2) {
    utils::seq_bytes bytes = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1,
                              0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1,
                              0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0,
                              0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0};
    double p = nist::runs_test(bytes);
    double answer = 0.500798;
    ASSERT_NEAR(p, answer, abs_error);
}
