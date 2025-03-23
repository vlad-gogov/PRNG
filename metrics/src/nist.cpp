#include "statistical_test/nist.hpp"

namespace statistical_test {

NistTest::NistTest(const utils::seq_bytes &bytes, const double &alpha) : StatisticalTest(bytes, alpha) {
}

void NistTest::test(const bool &print_p_values) const {
    std::double_t p_value = nist::frequency_test(bytes);
    std::cout << "Частотный побитовый тест: " << result_to_string(compare_p_value(p_value)) << std::endl;
    if (print_p_values) {
        std::cout << p_value << std::endl << std::endl;
    }

    p_value = nist::frequency_block_test(bytes, 128);
    std::cout << "Частотный блочный тест: " << result_to_string(compare_p_value(p_value)) << std::endl;
    if (print_p_values) {
        std::cout << p_value << std::endl << std::endl;
    }

    p_value = nist::runs_test(bytes);
    std::cout << "Тест на последовательность одинаковых бит: " << result_to_string(compare_p_value(p_value))
              << std::endl;
    if (print_p_values) {
        std::cout << p_value << std::endl << std::endl;
    }

    p_value = nist::longest_run_of_ones(bytes);
    std::cout << "Тест на самую длинную последовательность единиц в блоке: "
              << result_to_string(compare_p_value(p_value)) << std::endl;
    if (print_p_values) {
        std::cout << p_value << std::endl << std::endl;
    }

    p_value = nist::binary_matrix_rank(bytes, 32, 32);
    std::cout << "Тест рангов бинарных матриц: " << result_to_string(compare_p_value(p_value)) << std::endl;
    if (print_p_values) {
        std::cout << p_value << std::endl << std::endl;
    }

    p_value = nist::discrete_fourier_transform(bytes);
    std::cout << "Спектральный тест: " << result_to_string(compare_p_value(p_value)) << std::endl;
    if (print_p_values) {
        std::cout << p_value << std::endl << std::endl;
    }

    utils::seq_bytes template_ = {0, 0, 0, 0, 0, 0, 0, 0, 1};
    p_value = nist::non_overlapping_template_matching(bytes, template_);
    std::cout << "Тест на совпадение неперекрывающихся шаблонов: " << result_to_string(compare_p_value(p_value))
              << std::endl;
    if (print_p_values) {
        std::cout << p_value << std::endl << std::endl;
    }

    template_ = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    p_value = nist::overlapping_template_matching(bytes, template_);
    std::cout << "Тест на совпадение перекрывающихся шаблонов: " << result_to_string(compare_p_value(p_value))
              << std::endl;
    if (print_p_values) {
        std::cout << p_value << std::endl << std::endl;
    }

    p_value = nist::universal(bytes);
    std::cout << "Универсальный статистический тест Маурера: " << result_to_string(compare_p_value(p_value))
              << std::endl;
    if (print_p_values) {
        std::cout << p_value << std::endl << std::endl;
    }

    p_value = nist::linear_complexity(bytes, 1000);
    std::cout << "Тест на линейную сложность: " << result_to_string(compare_p_value(p_value)) << std::endl;
    if (print_p_values) {
        std::cout << p_value << std::endl << std::endl;
    }

    auto [p_value1, p_value2] = nist::serial_complexity(bytes, 16);
    std::cout << "Тест на периодичность: " << result_to_string(compare_p_value(p_value)) << std::endl;
    if (print_p_values) {
        std::cout << p_value1 << " " << p_value2 << std::endl << std::endl;
    }

    p_value = nist::approximate_entropy(bytes, 10);
    std::cout << "Тест приблизительной энтропии: " << result_to_string(compare_p_value(p_value)) << std::endl;
    if (print_p_values) {
        std::cout << p_value << std::endl << std::endl;
    }

    p_value = nist::cumulative_sums(bytes, nist::CumulativeSumsMode::Forward);
    std::cout << "Тест кумулятивных сумм: " << result_to_string(compare_p_value(p_value)) << std::endl;
    if (print_p_values) {
        std::cout << p_value << std::endl << std::endl;
    }

    std::vector<std::double_t> p_values = nist::random_excursions(bytes);
    bool result = true;
    for (const auto &p_value : p_values) {
        result &= compare_p_value(p_value);
    }
    std::cout << "Тест на произвольные отклонения: " << result_to_string(p_value) << std::endl;
    if (print_p_values) {
        for (const auto &p_value : p_values) {
            std::cout << p_value << " ";
        }
        std::cout << std::endl << std::endl;
    }

    p_values = nist::random_excursions_variant(bytes);
    result = true;
    for (const auto &p_value : p_values) {
        result &= compare_p_value(p_value);
    }
    std::cout << "Другой тест на произвольные отклонения: " << result_to_string(p_value) << std::endl;
    if (print_p_values) {
        for (const auto &p_value : p_values) {
            std::cout << p_value << " ";
        }
        std::cout << std::endl << std::endl;
    }
}

}; // namespace statistical_test
