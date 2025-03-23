#pragma once

#include <string_view>
#include <vector>

#include "metrics/utils.hpp"

namespace statistical_test {

std::string_view result_to_string(const bool value);

class StatisticalTest {
  protected:
    utils::seq_bytes bytes;
    std::double_t alpha;

    bool compare_p_value(const std::double_t &p_value) const;

  public:
    StatisticalTest(const utils::seq_bytes &bytes, const double &alpha);

    virtual void test(const bool &print_p_value) const = 0;
};
} // namespace statistical_test
