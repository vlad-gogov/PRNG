#pragma once

#include <string_view>
#include <vector>

#include "metrics/utils.hpp"

namespace statistical_test {

std::string_view result_to_string(const bool value);

class StatisticalTest {
  protected:
    std::double_t alpha;
    size_t test_count;

    bool compare_p_value(const std::double_t &p_value) const;

  public:
    StatisticalTest(const double &alpha);

    virtual void test(const utils::seq_bytes &bytes, const bool &print_p_value) = 0;
    virtual void print_statistics(const std::string &generator_name) const = 0;
};
} // namespace statistical_test
