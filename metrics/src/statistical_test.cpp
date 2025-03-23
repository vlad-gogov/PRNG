#include "statistical_test/statistical_test.hpp"

namespace statistical_test {

std::string_view result_to_string(const bool value) {
    return value ? "Pass" : "Fail";
}

bool StatisticalTest::compare_p_value(const std::double_t &p_value) const {
    return p_value >= alpha;
}

StatisticalTest::StatisticalTest(const utils::seq_bytes &bytes, const double &alpha) : bytes(bytes), alpha(alpha) {
}

}; // namespace statistical_test
