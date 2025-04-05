#pragma once

#include <array>
#include <string_view>

#include "metrics/nist_tests.hpp"
#include "statistical_test/statistical_test.hpp"

namespace statistical_test {

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

  public:
    NistTest(const double &alpha = 0.01f);

    void test(const utils::seq_bytes &bytes, const bool &print_p_values = false) override;

    void print_statistics() const override;
};

}; // namespace statistical_test
