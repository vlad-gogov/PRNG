#include "statistical_test/nist.hpp"

#include "metrics/utils.hpp"

#include <algorithm>
#include <filesystem>
#include <sstream>
#include <stdexcept>
#include <system_error>

#include <boost/math/special_functions/gamma.hpp>

namespace statistical_test {

NistTest::NistTest(const double &alpha)
    : StatisticalTest(alpha), test_success(40, 0), save_p_values(40), test_errors(15) {
}

void NistTest::test(const utils::seq_bytes &bytes, const bool &print_p_values) {
    ++test_count;
    std::string num_test = std::to_string(test_count) + ". ";
    {
        std::double_t p_value = nist::frequency_test(bytes);
        if (print_p_values) {
            std::cout << test_names[0] << ": " << p_value << std::endl;
        }
        save_p_values[0].push_back(p_value);
        bool res = compare_p_value(p_value);
        test_success[0] += res;
        if (!res) {
            for (size_t i = 1; i < 40; i++) {
                save_p_values[i].push_back(0.0);
                test_success[i] += false;
            }
            return;
        }
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
        try {
            std::double_t p_value = nist::binary_matrix_rank(bytes, 32, 32);
            if (print_p_values) {
                std::cout << test_names[4] << ": " << p_value << std::endl;
            }
            save_p_values[4].push_back(p_value);
            test_success[4] += compare_p_value(p_value);
        } catch (const std::runtime_error &e) {
            test_errors[4].push_back(num_test + e.what());
            save_p_values[4].push_back(0.0);
        }
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
        try {
            std::double_t p_value = nist::universal(bytes);
            if (print_p_values) {
                std::cout << test_names[8] << ": " << p_value << std::endl;
            }
            save_p_values[8].push_back(p_value);
            test_success[8] += compare_p_value(p_value);
        } catch (const std::runtime_error &e) {
            test_errors[8].push_back(num_test + e.what());
            save_p_values[8].push_back(0.0);
        }
    }

    {
        try {
            std::double_t p_value = nist::linear_complexity(bytes, 500);
            if (print_p_values) {
                std::cout << test_names[9] << ": " << p_value << std::endl;
            }
            save_p_values[9].push_back(p_value);
            test_success[9] += compare_p_value(p_value);
        } catch (const std::runtime_error &e) {
            test_errors[9].push_back(num_test + e.what());
            save_p_values[9].push_back(0.0);
        }
    }

    {
        auto [p_value1, p_value2] = nist::serial_complexity(bytes, 16);
        if (print_p_values) {
            std::cout << test_names[10] << ": " << p_value1 << " " << test_names[11] << ": " << p_value2 << std::endl;
        }
        save_p_values[10].push_back(p_value1);
        test_success[10] += compare_p_value(p_value1);
        save_p_values[11].push_back(p_value2);
        test_success[11] += compare_p_value(p_value2);
    }

    {
        std::double_t p_value = nist::approximate_entropy(bytes, 10);
        if (print_p_values) {
            std::cout << test_names[12] << ": " << p_value << std::endl;
        }
        save_p_values[12].push_back(p_value);
        test_success[12] += compare_p_value(p_value);
    }

    {
        std::double_t p_value = nist::cumulative_sums(bytes, nist::CumulativeSumsMode::Forward);
        if (print_p_values) {
            std::cout << test_names[13] << ": " << p_value << std::endl;
        }
        save_p_values[13].push_back(p_value);
        test_success[13] += compare_p_value(p_value);
    }

    {
        try {
            std::vector<std::double_t> p_values = nist::random_excursions(bytes);
            assert(p_values.size() == 8);
            for (int s = -4, i = 0; s <= 4; ++s) {
                if (s == 0) {
                    continue;
                }
                if (print_p_values) {
                    std::cout << test_names[14 + i] << ": p-value = " << p_values[i] << "\n";
                }
                save_p_values[14 + i].push_back(p_values[i]);
                test_success[14 + i] += compare_p_value(p_values[i]);
                i++;
            }
        } catch (const std::runtime_error &e) {
            test_errors[13].push_back(num_test + e.what());
            for (size_t i = 0; i < 18; ++i) {
                save_p_values[14 + i].push_back(0.0);
            }
        }
    }

    {
        try {
            std::vector<std::double_t> p_values = nist::random_excursions_variant(bytes);
            assert(p_values.size() == 18);
            for (int s = -9, i = 0; s <= 9; ++s) {
                if (s == 0) {
                    continue;
                }
                if (print_p_values) {
                    std::cout << test_names[22 + i] << ": p-value = " << p_values[i] << "\n";
                }
                save_p_values[22 + i].push_back(p_values[i]);
                test_success[22 + i] += compare_p_value(p_values[i]);
                i++;
            }
        } catch (const std::runtime_error &e) {
            test_errors[14].push_back(num_test + e.what());
            for (size_t i = 0; i < 18; ++i) {
                save_p_values[22 + i].push_back(0.0);
            }
        }
    }
}

void NistTest::print_statistics(const std::string &generator_name) const {
    constexpr std::size_t bins = 10;
    constexpr std::double_t bin_width = 0.1; // Choose your bin interval
    std::filesystem::path root_folder;
    try {
        std::error_code code;
        root_folder = std::filesystem::current_path(code).parent_path().parent_path();
        assert(!code);
    } catch (const std::filesystem::filesystem_error &err) {
        std::cout << err.what() << std::endl;
    }
    std::double_t temp = 3 * std::sqrt(alpha * (1 - alpha) / test_count * 1.0);
    std::double_t pass_value_max = 1 - alpha + temp;
    std::double_t pass_value_min = 1 - alpha - temp;
    size_t pass_count = 0;
    std::stringstream path_directory;
    path_directory << root_folder.c_str() << "/results/nist_test/" << generator_name << "_" << alpha;
    try {
        std::error_code code;
        std::filesystem::remove_all(path_directory.str(), code);
        assert(!code);
    } catch (const std::filesystem::filesystem_error &error) {
        std::cout << error.what() << std::endl;
    }
    try {
        std::error_code code;
        std::filesystem::create_directory(path_directory.str(), code);
        assert(!code);
    } catch (const std::filesystem::filesystem_error &error) {
        std::cout << error.what() << std::endl;
    }
    std::stringstream result;
    result << "Nist test for " << generator_name << " pass value: [" << pass_value_min << ";" << pass_value_max << "]"
           << std::endl;
    for (size_t i = 0; i < 40; ++i) {
        // pass test
        std::double_t p = static_cast<std::double_t>(test_success[i]) / static_cast<std::double_t>(test_count);
        bool answer_test = pass_value_min <= p && p <= pass_value_max;

        // uniform p-values
        bool answer_uniform = false;
        std::double_t p_uniform = 0.0;
        if (save_p_values[i].size() >= 55) {
            auto copy_p_values = save_p_values[i];
            std::sort(copy_p_values.begin(), copy_p_values.end());
            std::vector<uint32_t> histogram(10, 0);
            std::double_t bin = 0;
            std::uint32_t bin_count = 0;
            for (const auto &e : copy_p_values) {
                e >= bin + bin_width ? bin += bin_width, ++bin_count : false;
                ++histogram[bin_count];
            }
            std::cout << std::fixed << std::setprecision(6);
            std::double_t expected_p_values = test_count * 1.0 / bins;
            std::double_t chi = 0;
            for (size_t i = 0; i < bins; ++i) {
                chi += (histogram[i] * 1.0 - expected_p_values) * (histogram[i] * 1.0 - expected_p_values);
            }
            chi = chi / expected_p_values;
            p_uniform = boost::math::gamma_q(9 / 2.0, chi / 2.0);
            answer_uniform = p_uniform > 0.0001;
        }

        std::string copy_name = std::string(test_names[i]);
        std::replace(copy_name.begin(), copy_name.end(), ' ', '_');
        std::stringstream path;
        path << path_directory.str() << "/" << copy_name << ".txt";
        utils::save_p_values_to_file(path.str(), save_p_values[i]);

        result << test_names[i] << ": " << result_to_string(answer_test) << " (" << test_success[i] << " / "
               << test_count << ") && uniform p-values (" << p_uniform << ") " << result_to_string(answer_uniform)
               << " = " << result_to_string(answer_test & answer_uniform) << std::endl;
        pass_count += answer_test & answer_uniform;
    }
    result << "Pass test count: " << pass_count << std::endl;
    std::cout << result.str();
    utils::save_string_to_file(path_directory.str() + "/result.txt", result.str());
    for (size_t i = 0; i < 15; ++i) {
        if (!test_errors[i].empty()) {
            std::stringstream path;
            path << path_directory.str() << "/" << test_names_error[i] << "_errors.txt";
            for (const auto &e : test_errors[i]) {
                utils::save_string_to_file(path.str(), e + '\n', true);
            }
        }
    }
}

}; // namespace statistical_test
