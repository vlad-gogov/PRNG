#pragma once

#include <array>
#include <string_view>

#include "metrics/diehard_tests.hpp"
#include "statistical_test/statistical_test.hpp"

namespace statistical_test {

class diehardTest : private StatisticalTest {

    static constexpr std::array<std::string_view, 15> test_names = {"Runs test"
                                                                    "Matrix test"
                                                                    "Birthdays test"
                                                                    "Minimum distance test"
                                                                    "Overlapping permutations test"
                                                                    "Monkey test"
                                                                    "Squeeze test"
                                                                    "Sums test"
                                                                    "Craps test"};

    std::array<size_t, 15> test_success;

  public:
    DiehardTest(const double &alpha = 0.01f);

    void test(const utils::seq_bytes &bytes, const bool &print_p_values = false) override;

    void print_statistics() const override;
};

}; // namespace statistical_test
