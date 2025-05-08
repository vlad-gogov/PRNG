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

    static constexpr std::array<std::string_view, 15> test_names = {
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

    std::array<size_t, 15> test_success;
    std::array<std::vector<std::double_t>, 15> save_p_values;
    std::array<std::vector<std::string>, 15> test_errors;

  public:
    NistTest(const double &alpha = 0.01f);

    void test(const utils::seq_bytes &bytes, const bool &print_p_values = false) override;

    void print_statistics(const std::string &generator_name) const override;
};

}; // namespace statistical_test
