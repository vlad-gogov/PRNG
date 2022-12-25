#include <gtest/gtest.h>

#include "metrics/utils.hpp"

#include <iostream>

TEST(Utils, can_convert_number_to_seq_bytes_1) {
    std::uint32_t number = 100U;
    utils::seq_bytes seq = utils::convert_number_to_seq_bytes<std::uint32_t>(number);
    utils::seq_bytes answer = {1, 1, 0, 0, 1, 0, 0};
    ASSERT_EQ(seq, answer);
}

TEST(Utils, can_convert_number_to_seq_bytes_2) {
    std::uint32_t number = 0U;
    utils::seq_bytes seq = utils::convert_number_to_seq_bytes<std::uint32_t>(number);
    utils::seq_bytes answer = {0};
    ASSERT_EQ(seq, answer);
}

TEST(Utils, can_convert_number_to_seq_bytes_3) {
    std::uint32_t number = 1U;
    utils::seq_bytes seq = utils::convert_number_to_seq_bytes<std::uint32_t>(number);
    utils::seq_bytes answer = {1};
    ASSERT_EQ(seq, answer);
}

TEST(Utils, can_convert_number_to_seq_bytes_4) {
    std::uint32_t number = 4U;
    utils::seq_bytes seq = utils::convert_number_to_seq_bytes<std::uint32_t>(number);
    utils::seq_bytes answer = {1, 0, 0};
    ASSERT_EQ(seq, answer);
}

TEST(Utils, can_convert_number_to_seq_bytes_5) {
    std::uint32_t number = 16U;
    utils::seq_bytes seq = utils::convert_number_to_seq_bytes<std::uint32_t>(number);
    utils::seq_bytes answer = {1, 0, 0, 0, 0};
    ASSERT_EQ(seq, answer);
}

TEST(Utils, can_convert_number_to_seq_bytes_6) {
    std::uint32_t number = 12345U;
    utils::seq_bytes seq = utils::convert_number_to_seq_bytes<std::uint32_t>(number);
    utils::seq_bytes answer = {1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1};
    ASSERT_EQ(seq, answer);
}

TEST(Utils, can_convert_numbers_to_seq_bytes_1) {
    std::vector<std::uint32_t> numbers = {100U, 100U};
    utils::seq_bytes seq = utils::convert_numbers_to_seq_bytes<std::uint32_t>(numbers);
    utils::seq_bytes answer = {1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0};
    ASSERT_EQ(seq, answer);
}

TEST(Utils, can_convert_numbers_to_seq_bytes_2) {
    std::vector<std::uint32_t> numbers = {0U, 1U};
    utils::seq_bytes seq = utils::convert_numbers_to_seq_bytes<std::uint32_t>(numbers);
    utils::seq_bytes answer = {0, 1};
    ASSERT_EQ(seq, answer);
}

TEST(Utils, can_convert_numbers_to_seq_bytes_3) {
    std::vector<std::uint32_t> numbers = {1U, 4U, 16U, 12345U};
    utils::seq_bytes seq = utils::convert_numbers_to_seq_bytes<std::uint32_t>(numbers);
    utils::seq_bytes answer = {1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1};
    ASSERT_EQ(seq, answer);
}

TEST(Utils, can_get_max_run_1) {
    utils::seq_bytes seq = {1, 1, 0, 0, 1, 1, 0, 0};
    size_t our_answer = utils::get_max_run(seq);
    size_t answer = 2;
    ASSERT_EQ(our_answer, answer);
}

TEST(Utils, can_get_max_run_2) {
    utils::seq_bytes seq = {0, 1, 1, 0, 1, 1, 0, 0};
    size_t our_answer = utils::get_max_run(seq);
    size_t answer = 2;
    ASSERT_EQ(our_answer, answer);
}

TEST(Utils, can_get_max_run_3) {
    utils::seq_bytes seq = {1, 1, 1, 0, 0, 0, 0, 0};
    size_t our_answer = utils::get_max_run(seq);
    size_t answer = 3;
    ASSERT_EQ(our_answer, answer);
}

TEST(Utils, can_get_max_run_4) {
    utils::seq_bytes seq = {0, 1, 0, 1, 0, 0, 0, 1};
    size_t our_answer = utils::get_max_run(seq);
    size_t answer = 1;
    ASSERT_EQ(our_answer, answer);
}

TEST(Utils, can_get_max_run_5) {
    utils::seq_bytes seq = {1, 0, 0, 0, 0, 0, 0, 0};
    size_t our_answer = utils::get_max_run(seq);
    size_t answer = 1;
    ASSERT_EQ(our_answer, answer);
}

TEST(Utils, can_get_max_run_6) {
    utils::seq_bytes seq = {1, 1, 0, 1, 0, 1, 1, 1};
    size_t our_answer = utils::get_max_run(seq);
    size_t answer = 3;
    ASSERT_EQ(our_answer, answer);
}

TEST(Utils, can_get_max_run_7) {
    utils::seq_bytes seq = {0, 0, 0, 0, 0, 0, 1, 0};
    size_t our_answer = utils::get_max_run(seq);
    size_t answer = 1;
    ASSERT_EQ(our_answer, answer);
}
