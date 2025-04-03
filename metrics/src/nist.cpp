#include "statistical_test/nist.hpp"

namespace statistical_test {

NistTest::NistTest(const std::vector<utils::seq_bytes> &bytes, const double &alpha) : StatisticalTest(bytes, alpha) {
}

void NistTest::test(const bool &print_p_values) const {
    std::double_t success = 0;
    for (size_t i = 0; i < bytes.size(); ++i) {
        std::double_t p_value = nist::frequency_test(bytes[i]);
        if (print_p_values) {
            std::cout << p_value << std::endl << std::endl;
        }
        success += compare_p_value(p_value);
    }
    std::cout << "Частотный побитовый тест: " << result_to_string(success / bytes.size() >= 0.95) << " (" << success
              << " / " << bytes.size() << ")" << std::endl;

    success = 0;
    for (size_t i = 0; i < bytes.size(); ++i) {
        std::double_t p_value = nist::frequency_block_test(bytes[i], 128);
        if (print_p_values) {
            std::cout << p_value << std::endl << std::endl;
        }
        success += compare_p_value(p_value);
    }
    std::cout << "Частотный блочный тест: " << result_to_string(success / bytes.size() >= 0.95) << " (" << success
              << " / " << bytes.size() << ")" << std::endl;

    success = 0;
    for (size_t i = 0; i < bytes.size(); ++i) {
        std::double_t p_value = nist::runs_test(bytes[i]);
        if (print_p_values) {
            std::cout << p_value << std::endl << std::endl;
        }
        success += compare_p_value(p_value);
    }
    std::cout << "Тест на последовательность одинаковых бит: " << result_to_string(success / bytes.size() >= 0.95)
              << " (" << success << " / " << bytes.size() << ")" << std::endl;

    success = 0;
    for (size_t i = 0; i < bytes.size(); ++i) {
        std::double_t p_value = nist::longest_run_of_ones(bytes[i]);
        if (print_p_values) {
            std::cout << p_value << std::endl << std::endl;
        }
        success += compare_p_value(p_value);
    }
    std::cout << "Тест на самую длинную последовательность единиц в блоке: "
              << result_to_string(success / bytes.size() >= 0.95) << " (" << success << " / " << bytes.size() << ")"
              << std::endl;

    success = 0;
    for (size_t i = 0; i < bytes.size(); ++i) {
        std::double_t p_value = nist::binary_matrix_rank(bytes[i], 32, 32);
        if (print_p_values) {
            std::cout << p_value << std::endl << std::endl;
        }
        success += compare_p_value(p_value);
    }
    std::cout << "Тест рангов бинарных матриц: " << result_to_string(success / bytes.size() >= 0.95) << " (" << success
              << " / " << bytes.size() << ")" << std::endl;

    success = 0;
    for (size_t i = 0; i < bytes.size(); ++i) {
        std::double_t p_value = nist::discrete_fourier_transform(bytes[i]);
        if (print_p_values) {
            std::cout << p_value << std::endl << std::endl;
        }
        success += compare_p_value(p_value);
    }
    std::cout << "Спектральный тест: " << result_to_string(success / bytes.size() >= 0.95) << " (" << success << " / "
              << bytes.size() << ")" << std::endl;

    utils::seq_bytes template_ = {0, 0, 0, 0, 0, 0, 0, 0, 1};
    success = 0;
    for (size_t i = 0; i < bytes.size(); ++i) {
        std::double_t p_value = nist::non_overlapping_template_matching(bytes[i], template_);
        if (print_p_values) {
            std::cout << p_value << std::endl << std::endl;
        }
        success += compare_p_value(p_value);
    }
    std::cout << "Тест на совпадение неперекрывающихся шаблонов: " << result_to_string(success / bytes.size() >= 0.95)
              << " (" << success << " / " << bytes.size() << ")" << std::endl;

    template_ = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    success = 0;
    for (size_t i = 0; i < bytes.size(); ++i) {
        std::double_t p_value = nist::overlapping_template_matching(bytes[i], template_);
        if (print_p_values) {
            std::cout << p_value << std::endl << std::endl;
        }
        success += compare_p_value(p_value);
    }
    std::cout << "Тест на совпадение перекрывающихся шаблонов: " << result_to_string(success / bytes.size() >= 0.95)
              << " (" << success << " / " << bytes.size() << ")" << std::endl;

    success = 0;
    for (size_t i = 0; i < bytes.size(); ++i) {
        std::double_t p_value = nist::universal(bytes[i]);
        if (print_p_values) {
            std::cout << p_value << std::endl << std::endl;
        }
        success += compare_p_value(p_value);
    }
    std::cout << "Универсальный статистический тест Маурера: " << result_to_string(success / bytes.size() >= 0.95)
              << " (" << success << " / " << bytes.size() << ")" << std::endl;

    success = 0;
    for (size_t i = 0; i < bytes.size(); ++i) {
        std::double_t p_value = nist::linear_complexity(bytes[i], 1000);
        if (print_p_values) {
            std::cout << p_value << std::endl << std::endl;
        }
        success += compare_p_value(p_value);
    }
    std::cout << "Тест на линейную сложность: " << result_to_string(success / bytes.size() >= 0.95) << " (" << success
              << " / " << bytes.size() << ")" << std::endl;

    success = 0;
    for (size_t i = 0; i < bytes.size(); ++i) {
        auto [p_value1, p_value2] = nist::serial_complexity(bytes[i], 16);
        if (print_p_values) {
            std::cout << p_value1 << " " << p_value2 << std::endl << std::endl;
        }
        success += compare_p_value(p_value1) && compare_p_value(p_value2);
    }
    std::cout << "Тест на периодичность: " << result_to_string(success / bytes.size() >= 0.95) << " (" << success
              << " / " << bytes.size() << ")" << std::endl;

    success = 0;
    for (size_t i = 0; i < bytes.size(); ++i) {
        std::double_t p_value = nist::approximate_entropy(bytes[i], 10);
        if (print_p_values) {
            std::cout << p_value << std::endl << std::endl;
        }
        success += compare_p_value(p_value);
    }
    std::cout << "Тест приблизительной энтропии: " << result_to_string(success / bytes.size() >= 0.95) << " ("
              << success << " / " << bytes.size() << ")" << std::endl;

    success = 0;
    for (size_t i = 0; i < bytes.size(); ++i) {
        std::double_t p_value = nist::cumulative_sums(bytes[i], nist::CumulativeSumsMode::Forward);
        if (print_p_values) {
            std::cout << p_value << std::endl << std::endl;
        }
        success += compare_p_value(p_value);
    }
    std::cout << "Тест кумулятивных сумм: " << result_to_string(success / bytes.size() >= 0.95) << " (" << success
              << " / " << bytes.size() << ")" << std::endl;

    success = 0;
    for (size_t i = 0; i < bytes.size(); ++i) {
        std::vector<std::double_t> p_values = nist::random_excursions(bytes[i]);
        bool result = true;
        for (const auto &p_value : p_values) {
            result &= compare_p_value(p_value);
        }
        if (print_p_values) {
            for (const auto &p_value : p_values) {
                std::cout << p_value << " ";
            }
            std::cout << std::endl << std::endl;
        }
        success += result;
    }
    std::cout << "Тест на произвольные отклонения: " << result_to_string(success / bytes.size() >= 0.95) << "("
              << success << " / " << bytes.size() << ")" << std::endl;

    success = 0;
    for (size_t i = 0; i < bytes.size(); ++i) {
        std::vector<std::double_t> p_values = nist::random_excursions_variant(bytes[i]);
        bool result = true;
        for (const auto &p_value : p_values) {
            result &= compare_p_value(p_value);
        }
        if (print_p_values) {
            for (const auto &p_value : p_values) {
                std::cout << p_value << " ";
            }
            std::cout << std::endl << std::endl;
        }
        success += result;
    }
    std::cout << "Другой тест на произвольные отклонения: " << result_to_string(success / bytes.size() >= 0.95) << "("
              << success << " / " << bytes.size() << ")" << std::endl;
}

}; // namespace statistical_test
