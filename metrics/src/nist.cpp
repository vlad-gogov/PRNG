#include "statistical_test/nist.hpp"

namespace statistical_test {

NistTest::NistTest(const double &alpha) : StatisticalTest(alpha) {
    test_success.fill(0);
}

void NistTest::test(const utils::seq_bytes &bytes, const bool &print_p_values) {
    test_count++;
    {
        std::double_t p_value = nist::frequency_test(bytes);
        if (print_p_values) {
            std::cout << test_names[0] << ": " << p_value << std::endl;
        }
        test_success[0] += compare_p_value(p_value);
    }

    {
        std::double_t p_value = nist::frequency_block_test(bytes, 128);
        if (print_p_values) {
            std::cout << test_names[1] << ": " << p_value << std::endl;
        }
        test_success[1] += compare_p_value(p_value);
    }

    {
        std::double_t p_value = nist::runs_test(bytes);
        if (print_p_values) {
            std::cout << test_names[2] << ": " << p_value << std::endl;
        }
        test_success[2] += compare_p_value(p_value);
    }

    {
        std::double_t p_value = nist::longest_run_of_ones(bytes);
        if (print_p_values) {
            std::cout << test_names[3] << ": " << p_value << std::endl;
        }
        test_success[3] += compare_p_value(p_value);
    }

    {
        std::double_t p_value = nist::binary_matrix_rank(bytes, 32, 32);
        if (print_p_values) {
            std::cout << test_names[4] << ": " << p_value << std::endl;
        }
        test_success[4] += compare_p_value(p_value);
    }

    {
        std::double_t p_value = nist::discrete_fourier_transform(bytes);
        if (print_p_values) {
            std::cout << test_names[5] << ": " << p_value << std::endl;
        }
        test_success[5] += compare_p_value(p_value);
    }

    {
        utils::seq_bytes template_ = {0, 0, 0, 0, 0, 0, 0, 0, 1};
        std::double_t p_value = nist::non_overlapping_template_matching(bytes, template_);
        if (print_p_values) {
            std::cout << test_names[6] << ": " << p_value << std::endl;
        }
        test_success[6] += compare_p_value(p_value);
    }

    {
        utils::seq_bytes template_ = {1, 1, 1, 1, 1, 1, 1, 1, 1};
        std::double_t p_value = nist::overlapping_template_matching(bytes, template_);
        if (print_p_values) {
            std::cout << test_names[7] << ": " << p_value << std::endl;
        }
        test_success[7] += compare_p_value(p_value);
    }

    {
        std::double_t p_value = nist::universal(bytes);
        if (print_p_values) {
            std::cout << test_names[8] << ": " << p_value << std::endl;
        }
        test_success[8] += compare_p_value(p_value);
    }

    {
        std::double_t p_value = nist::linear_complexity(bytes, 1000);
        if (print_p_values) {
            std::cout << test_names[9] << ": " << p_value << std::endl;
        }
        test_success[9] += compare_p_value(p_value);
    }

    {
        auto [p_value1, p_value2] = nist::serial_complexity(bytes, 16);
        if (print_p_values) {
            std::cout << test_success[10] << ": " << p_value1 << " " << p_value2 << std::endl;
        }
        test_success[10] += compare_p_value(p_value1) && compare_p_value(p_value2);
    }

    {
        std::double_t p_value = nist::approximate_entropy(bytes, 10);
        if (print_p_values) {
            std::cout << test_names[11] << ": " << p_value << std::endl;
        }
        test_success[11] += compare_p_value(p_value);
    }

    {
        std::double_t p_value = nist::cumulative_sums(bytes, nist::CumulativeSumsMode::Forward);
        if (print_p_values) {
            std::cout << test_names[12] << ": " << p_value << std::endl;
        }
        test_success[12] += compare_p_value(p_value);
    }

    {
        std::vector<std::double_t> p_values = nist::random_excursions(bytes);
        bool result = true;
        for (const auto &p_value : p_values) {
            result &= compare_p_value(p_value);
        }
        if (print_p_values) {
            std::cout << test_names[13] << ": ";
            for (const auto &p_value : p_values) {
                std::cout << p_value << " ";
            }
            std::cout << std::endl;
        }
        test_success[13] += result;
    }

    {
        std::vector<std::double_t> p_values = nist::random_excursions_variant(bytes);
        bool result = true;
        for (const auto &p_value : p_values) {
            result &= compare_p_value(p_value);
        }
        if (print_p_values) {
            std::cout << test_names[14] << ": ";
            for (const auto &p_value : p_values) {
                std::cout << p_value << " ";
            }
            std::cout << std::endl;
        }
        test_success[14] += result;
    }
}

void NistTest::print_statistics(const std::string &generator_name) const {
    std::cout << "Nist test for " << generator_name << std::endl;
    size_t pass_count = 0;
    for (size_t i = 0; i < 15; ++i) {
        bool answer = static_cast<std::double_t>(test_success[i]) / static_cast<std::double_t>(test_count) >= 0.95;
        std::cout << test_names[i] << ": " << result_to_string(answer) << " (" << test_success[i] << " / " << test_count
                  << ")" << std::endl;
        pass_count += answer;
    }
    std::cout << "Pass test count: " << pass_count << std::endl;
}

}; // namespace statistical_test
