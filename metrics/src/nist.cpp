#include "statistical_test/nist.hpp"

#include "metrics/utils.hpp"

#include <algorithm>
#include <filesystem>
#include <strstream>

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
        save_p_values[0].push_back(p_value);
        bool res = compare_p_value(p_value);
        if (!res) {
            return;
        }
        test_success[0] += res;
    }

    {
        std::double_t p_value = nist::frequency_block_test(bytes, 128);
        if (print_p_values) {
            std::cout << test_names[1] << ": " << p_value << std::endl;
        }
        save_p_values[1].push_back(p_value);
        test_success[1] += compare_p_value(p_value);
    }

    {
        std::double_t p_value = nist::runs_test(bytes);
        if (print_p_values) {
            std::cout << test_names[2] << ": " << p_value << std::endl;
        }
        save_p_values[2].push_back(p_value);
        test_success[2] += compare_p_value(p_value);
    }

    {
        std::double_t p_value = nist::longest_run_of_ones(bytes);
        if (print_p_values) {
            std::cout << test_names[3] << ": " << p_value << std::endl;
        }
        save_p_values[3].push_back(p_value);
        test_success[3] += compare_p_value(p_value);
    }

    {
        std::double_t p_value = nist::binary_matrix_rank(bytes, 32, 32);
        if (print_p_values) {
            std::cout << test_names[4] << ": " << p_value << std::endl;
        }
        save_p_values[4].push_back(p_value);
        test_success[4] += compare_p_value(p_value);
    }

    {
        std::double_t p_value = nist::discrete_fourier_transform(bytes);
        if (print_p_values) {
            std::cout << test_names[5] << ": " << p_value << std::endl;
        }
        save_p_values[5].push_back(p_value);
        test_success[5] += compare_p_value(p_value);
    }

    {
        utils::seq_bytes template_ = {0, 0, 0, 0, 0, 0, 0, 0, 1};
        std::double_t p_value = nist::non_overlapping_template_matching(bytes, template_);
        if (print_p_values) {
            std::cout << test_names[6] << ": " << p_value << std::endl;
        }
        save_p_values[6].push_back(p_value);
        test_success[6] += compare_p_value(p_value);
    }

    {
        utils::seq_bytes template_ = {1, 1, 1, 1, 1, 1, 1, 1, 1};
        std::double_t p_value = nist::overlapping_template_matching(bytes, template_);
        if (print_p_values) {
            std::cout << test_names[7] << ": " << p_value << std::endl;
        }
        save_p_values[7].push_back(p_value);
        test_success[7] += compare_p_value(p_value);
    }

    {
        std::double_t p_value = nist::universal(bytes);
        if (print_p_values) {
            std::cout << test_names[8] << ": " << p_value << std::endl;
        }
        save_p_values[8].push_back(p_value);
        test_success[8] += compare_p_value(p_value);
    }

    {
        std::double_t p_value = nist::linear_complexity(bytes, 1000);
        if (print_p_values) {
            std::cout << test_names[9] << ": " << p_value << std::endl;
        }
        save_p_values[9].push_back(p_value);
        test_success[9] += compare_p_value(p_value);
    }

    {
        auto [p_value1, p_value2] = nist::serial_complexity(bytes, 16);
        if (print_p_values) {
            std::cout << test_success[10] << ": " << p_value1 << " " << p_value2 << std::endl;
        }
        save_p_values[10].push_back(p_value1);
        save_p_values[10].push_back(p_value2);
        test_success[10] += compare_p_value(p_value1) && compare_p_value(p_value2);
    }

    {
        std::double_t p_value = nist::approximate_entropy(bytes, 10);
        if (print_p_values) {
            std::cout << test_names[11] << ": " << p_value << std::endl;
        }
        save_p_values[11].push_back(p_value);
        test_success[11] += compare_p_value(p_value);
    }

    {
        std::double_t p_value = nist::cumulative_sums(bytes, nist::CumulativeSumsMode::Forward);
        if (print_p_values) {
            std::cout << test_names[12] << ": " << p_value << std::endl;
        }
        save_p_values[12].push_back(p_value);
        test_success[12] += compare_p_value(p_value);
    }

    {
        std::vector<std::double_t> p_values = nist::random_excursions(bytes);
        bool result = true;
        for (const auto &p_value : p_values) {
            result &= compare_p_value(p_value);
        }
        for (const auto &p_value : p_values) {
            save_p_values[13].push_back(p_value);
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
        for (const auto &p_value : p_values) {
            save_p_values[14].push_back(p_value);
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
    auto root_folder = std::filesystem::current_path().parent_path().parent_path();
    std::double_t pass_value = 1 - alpha - 3 * std::sqrt(alpha * (1 - alpha) / test_count);
    std::cout << "Nist test for " << generator_name << " pass value: " << pass_value << std::endl;
    size_t pass_count = 0;
    for (size_t i = 0; i < 15; ++i) {
        bool answer =
            static_cast<std::double_t>(test_success[i]) / static_cast<std::double_t>(test_count) >= pass_value;
        std::cout << test_names[i] << ": " << result_to_string(answer) << " (" << test_success[i] << " / " << test_count
                  << ")" << std::endl;
        pass_count += answer;
        std::string copy_name = std::string(test_names[i]);
        std::replace(copy_name.begin(), copy_name.end(), ' ', '_');
        std::stringstream path;
        path << root_folder.c_str() << "/results/nist_test/" << generator_name;
        std::filesystem::create_directory(path.str());
        path << "/" << copy_name << ".txt";
        utils::save_p_values_to_file(path.str(), save_p_values[i]);
    }
    std::cout << "Pass test count: " << pass_count << std::endl;
}

}; // namespace statistical_test
