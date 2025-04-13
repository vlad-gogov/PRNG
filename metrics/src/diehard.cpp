#include "statistical_test/diehard.hpp"

namespace statistical_test {

DiehardTest::DiehardTest(const double &alpha) : StatisticalTest(alpha) {
    test_success.fill(0);
}

void DiehardTest::test(const utils::seq_bytes &bytes, const bool &print_p_values) {
    test_count++;
    {
        std::double_t p_value = diehard::runs_test(bytes);
        if (print_p_values) {
            std::cout << test_names[0] << ": " << p_value << std::endl;
        }
        test_success[0] += compare_p_value(p_value);
    }

    {
        std::double_t p_value = diehard::matrix_test(bytes, 32, 32, 100000 / (32 * 32));
        if (print_p_values) {
            std::cout << test_names[1] << ": " << p_value << std::endl;
        }
        test_success[1] += compare_p_value(p_value);
    }

    {
        std::double_t p_value = diehard::birthdays_test(bytes, 24, 52, 100);
        if (print_p_values) {
            std::cout << test_names[2] << ": " << p_value << std::endl;
        }
        test_success[2] += compare_p_value(p_value);
    }

    {
        std::double_t p_value = diehard::minimum_distance_test(bytes, 2, 800, 10);
        if (print_p_values) {
            std::cout << test_names[3] << ": " << p_value << std::endl;
        }
        test_success[3] += compare_p_value(p_value);
    }

    {
        std::double_t p_value = diehard::minimum_distance_test(bytes, 3, 400, 10);
        if (print_p_values) {
            std::cout << test_names[3] << ": " << p_value << std::endl;
        }
        test_success[4] += compare_p_value(p_value);
    }

    {
        std::double_t p_value = diehard::overlapping_permutations_test(bytes, 1000);
        if (print_p_values) {
            std::cout << test_names[4] << ": " << p_value << std::endl;
        }
        test_success[5] += compare_p_value(p_value);
    }

    {
        std::double_t p_value = diehard::monkey_test(bytes, 256000);
        if (print_p_values) {
            std::cout << test_names[5] << ": " << p_value << std::endl;
        }
        test_success[6] += compare_p_value(p_value);
    }

    {
        std::double_t p_value = diehard::squeeze_test(bytes, 200);
        if (print_p_values) {
            std::cout << test_names[6] << ": " << p_value << std::endl;
        }
        test_success[7] += compare_p_value(p_value);
    }

    {
        std::double_t p_value = diehard::sums_test(bytes, 10000);
        if (print_p_values) {
            std::cout << test_names[7] << ": " << p_value << std::endl;
        }
        test_success[8] += compare_p_value(p_value);
    }

    {
        std::double_t p_value = diehard::craps_test(bytes, 2000);
        if (print_p_values) {
            std::cout << test_names[8] << ": " << p_value << std::endl;
        }
        test_success[9] += compare_p_value(p_value);
    }
}

void DiehardTest::print_statistics(const std::string &generator_name) const {
    std::cout << "Diehard test for " << generator_name << std::endl;
    size_t pass_count = 0;
    for (size_t i = 0; i < 10; ++i) {
        bool answer = static_cast<std::double_t>(test_success[i]) / static_cast<std::double_t>(test_count) >= 0.95;
        std::cout << test_names[i] << ": " << result_to_string(answer) << " (" << test_success[i] << " / " << test_count
                  << ")" << std::endl;
        pass_count += answer;
    }
    std::cout << "Pass test count: " << pass_count << std::endl;
}

}; // namespace statistical_test
