#pragma once

#include <array>
#include <string>
#include <string_view>
#include <vector>

#include "metrics/nist_tests.hpp"
#include "statistical_test/statistical_test.hpp"

namespace statistical_test {

// min 1'000'000'000 bits = 125'000'000 bytes
class NistTest : private StatisticalTest {

    static constexpr std::array<std::string_view, 40> test_names = {
        "Frequency Test",
        "Frequency Block Test",
        "Runs Test",
        "Longest Run of Ones Test",
        "Binary Matrix Rank Test",
        "Discrete Fourier Transform Test",
        "Non-overlapping Template Matching Test",
        "Overlapping Template Matching Test",
        "Maurer’s “Universal Statistical” Test",
        "Linear Complexity Test",
        "Serial Test 16",
        "Serial Test 15",
        "Approximate Entropy Test",
        "Cumulative Sums Test",
        "Random Excursions Test -4",
        "Random Excursions Test -3",
        "Random Excursions Test -2",
        "Random Excursions Test -1",
        "Random Excursions Test 1",
        "Random Excursions Test 2",
        "Random Excursions Test 3",
        "Random Excursions Test 4",
        "Random Excursions Variant Test -9",
        "Random Excursions Variant Test -9",
        "Random Excursions Variant Test -7",
        "Random Excursions Variant Test -6",
        "Random Excursions Variant Test -5",
        "Random Excursions Variant Test -4",
        "Random Excursions Variant Test -3",
        "Random Excursions Variant Test -2",
        "Random Excursions Variant Test -1",
        "Random Excursions Variant Test 1",
        "Random Excursions Variant Test 2",
        "Random Excursions Variant Test 3",
        "Random Excursions Variant Test 4",
        "Random Excursions Variant Test 5",
        "Random Excursions Variant Test 6",
        "Random Excursions Variant Test 7",
        "Random Excursions Variant Test 8",
        "Random Excursions Variant Test 9",
    };

    static constexpr std::array<std::string_view, 15> test_names_error = {
        "Frequency Test",
        "Frequency Block Test",
        "Runs Test",
        "Longest Run of Ones Test",
        "Binary Matrix Rank Test",
        "Discrete Fourier Transform Test",
        "Non-overlapping Template Matching Test",
        "Overlapping Template Matching Test",
        "Maurer’s “Universal Statistical” Test",
        "Linear Complexity Test",
        "Serial Test",
        "Approximate Entropy Test",
        "Cumulative Sums Test",
        "Random Excursions Test",
        "Random Excursions Variant Test",
    };

    std::array<size_t, 40> test_success;
    std::array<std::vector<std::double_t>, 40> save_p_values;
    std::array<std::vector<std::string>, 15> test_errors;

  public:
    NistTest(const double &alpha = 0.01f);

    void test(const utils::seq_bytes &bytes, const bool &print_p_values = false) override;

    void print_statistics(const std::string &generator_name) const override;
};

}; // namespace statistical_test
