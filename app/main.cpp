#include <iostream>

#include "generators/linear_congruential_generator.hpp"
#include "metrics/nist_tests.hpp"

void nist_test() {
    constexpr std::uint32_t a = 65539U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    constexpr std::uint32_t seed = 1U;
    // constexpr std::uint32_t a = 5U;
    // constexpr std::uint32_t c = 55U;
    // constexpr std::uint32_t m = 100;
    // constexpr std::uint32_t seed = 1U;
    // constexpr std::uint32_t a = 16807U;
    // constexpr std::uint32_t c = 0U;
    // constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    // constexpr std::uint32_t seed = 23482349U;
    std::uint32_t count_number = 50;
    std::uint32_t discard = count_number * 2;
    std::cout << '(' << a << " * x_n + " << c << ')' << " * mod(" << m << ')' << ", seed = " << seed
              << ", discard = " << discard << std::endl;
    LinearCongruentialGenerator<std::uint32_t, a, c, m> generator(seed);
    generator.discard(discard);
    std::vector<std::uint32_t> numbers(count_number);
    std::cout << "Numbers: ";
    for (std::uint32_t i = 0; i < count_number; ++i) {
        numbers[i] = generator();
        std::cout << numbers[i] << ", ";
    }
    std::cout << std::endl << std::endl;
    utils::seq_bytes bytes = utils::convert_numbers_to_seq_bytes(numbers);

    auto result = nist::check_frequency_test(bytes);
    std::cout << "Частотный побитовый тест: " << result << std::endl;

    result = nist::check_frequency_block_test(bytes, 128);
    std::cout << "Частотный блочный тест: " << result << std::endl;

    result = nist::check_runs_test(bytes);
    std::cout << "Тест на последовательность одинаковых бит: " << result << std::endl;

    result = nist::check_longest_run_of_ones(bytes);
    std::cout << "Тест на самую длинную последовательность единиц в блоке: " << result << std::endl;

    result = nist::check_binary_matrix_rank(bytes, 32, 32);
    std::cout << "Тест рангов бинарных матриц: " << result << std::endl;

    result = nist::check_discrete_fourier_transform(bytes);
    std::cout << "Спектральный тест: " << result << std::endl;

    utils::seq_bytes template_ = {0, 0, 0, 0, 0, 0, 0, 0, 1};
    result = nist::check_non_overlapping_template_matching(bytes, template_);
    std::cout << "Тест на совпадение неперекрывающихся шаблонов: " << result << std::endl;

    template_ = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    result = nist::check_overlapping_template_matching(bytes, template_);
    std::cout << "Тест на совпадение перекрывающихся шаблонов: " << result << std::endl;

    result = nist::check_universal(bytes);
    std::cout << "Универсальный статистический тест Маурера: " << result << std::endl;

    result = nist::check_linear_complexity(bytes, 1000);
    std::cout << "Тест на линейную сложность: " << result << std::endl;

    result = nist::check_serial_complexity(bytes, 16);
    std::cout << "Тест на периодичность: " << result << std::endl;

    result = nist::check_approximate_entropy(bytes, 10);
    std::cout << "Тест приблизительной энтропии: " << result << std::endl;

    result = nist::check_cumulative_sums(bytes, nist::CumulativeSumsMode::Forward);
    std::cout << "Тест кумулятивных сумм: " << result << std::endl;

    auto result1 = nist::check_random_excursions(bytes);
    std::cout << "Тест на произвольные отклонения: " << std::endl;
    for (const auto &elem : result1) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    result1 = nist::check_random_excursions_variant(bytes);
    std::cout << "Другой тест на произвольные отклонения: " << std::endl;
    for (const auto &elem : result1) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

int main() {
    nist_test();
    return 0;
}