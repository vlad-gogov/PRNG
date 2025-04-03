#pragma once

#include "metrics/nist_tests.hpp"
#include "statistical_test/statistical_test.hpp"

namespace statistical_test {

class NistTest : private StatisticalTest {
  public:
    NistTest(const std::vector<utils::seq_bytes> &bytes, const double &alpha = 0.01f);

    void test(const bool &print_p_values = false) const override;
};

}; // namespace statistical_test
