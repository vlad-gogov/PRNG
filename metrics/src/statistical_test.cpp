#include "statistical_test/statistical_test.hpp"

namespace statistical_test {

std::string_view result_to_string(const bool value) {
    return value ? "Pass" : "Fail";
}

bool StatisticalTest::compare_p_value(const std::double_t &p_value) const {
    return p_value >= alpha;
}

StatisticalTest::StatisticalTest(const double &alpha) : alpha(alpha), test_count(0) {
    if (alpha < 0.0 || alpha > 1.0) {
        throw std::invalid_argument("Alpha must be in the range [0, 1]");
    }
}

}; // namespace statistical_test
