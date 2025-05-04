#pragma once

#include "indicators.hpp"

template <typename StatisticalTest, typename Generator>
void run_statistical_test(const std::string &generator_name, const size_t count_tests, const size_t count_number,
                          const uint32_t start_seed = 0u) {
    std::float_t progress = 0.0f;
    const std::float_t step_size = 100.0f / count_tests;

    indicators::show_console_cursor(false);

    indicators::BlockProgressBar bar{
        indicators::option::BarWidth{80},
        indicators::option::PrefixText{std::string("Nist tests ") + generator_name},
        indicators::option::Start{" ["},
        indicators::option::End{"]"},
        indicators::option::ForegroundColor{indicators::Color::green},
        indicators::option::ShowElapsedTime{true},
        indicators::option::ShowRemainingTime{true},
        indicators::option::ShowElapsedTime{true},
        indicators::option::FontStyles{std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}};

    StatisticalTest test;
    for (size_t i = 0; i < count_tests; ++i) {
        Generator generator(start_seed + i);
        std::vector<typename Generator::result_type> numbers(count_number);
        for (size_t j = 0; j < count_number; ++j) {
            numbers[j] = generator();
        }
        utils::seq_bytes bytes = utils::convert_numbers_to_seq_bytes(numbers);
        assert(bytes.size() == count_number * 32);
        test.test(bytes);
        progress += step_size;
        bar.set_progress(progress);
    }
    bar.mark_as_completed();
    indicators::show_console_cursor(true);

    test.print_statistics(generator_name);
}

template <typename Generator>
void print_gen_value(const size_t count_number, const typename Generator::result_type seed = 0u) {
    Generator generator(seed);
    for (size_t i = 0; i < count_number; ++i) {
        std::cout << generator() << " ";
    }
    std::cout << std::endl;
}
