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
        assert(bytes.size() == count_number * std::numeric_limits<typename Generator::result_type>::digits);
        test.test(bytes);
        progress += step_size;
        bar.set_progress(progress);
    }
    bar.mark_as_completed();
    indicators::show_console_cursor(true);

    test.print_statistics(generator_name);
}

template <typename StatisticalTest, typename Generator>
void run_statistical_test_without_progress_bar(const std::string &generator_name, const size_t count_tests,
                                               const size_t count_number, const uint32_t start_seed = 0u) {
    StatisticalTest test;
    for (size_t i = 0; i < count_tests; ++i) {
        Generator generator(start_seed + i);
        std::vector<typename Generator::result_type> numbers(count_number);
        for (size_t j = 0; j < count_number; ++j) {
            numbers[j] = generator();
        }
        utils::seq_bytes bytes = utils::convert_numbers_to_seq_bytes(numbers);
        assert(bytes.size() == count_number * std::numeric_limits<typename Generator::result_type>::digits);
        test.test(bytes);
        if ((i + 1) % 100 == 0) {
            std::cout << "Test " << i + 1 << " of " << count_tests << " completed." << std::endl;
        }
    }
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

template <typename Generator>
void ratio_one_zero(const size_t count_number, const typename Generator::result_type seed = 0u) {
    Generator generator(seed);
    std::vector<typename Generator::result_type> numbers(count_number);
    for (size_t i = 0; i < count_number; ++i) {
        numbers[i] = generator();
    }
    utils::seq_bytes bytes = utils::convert_numbers_to_seq_bytes(numbers);
    assert(bytes.size() == count_number * 32);
    size_t count_ones = 0;
    size_t count_zeros = 0;
    for (const auto &byte : bytes) {
        if (byte == 0) {
            count_zeros++;
        } else {
            count_ones++;
        }
    }
    std::cout << "Count zeros: " << count_zeros << std::endl;
    std::cout << "Count ones: " << count_ones << std::endl;
    std::cout << "Ratio zeros: " << static_cast<double>(count_zeros) / (count_number * 32) << std::endl;
    std::cout << "Ratio ones: " << static_cast<double>(count_ones) / (count_number * 32) << std::endl;
}

template <typename Generator1, typename Generator2>
void run_benchmark(const std::string generator1_name, const std::string generator2_name, const size_t count_number,
                   const typename Generator2::result_type seed = 0u, const size_t warmup_count = 0) {
    std::cout << "Benchmark count numbers: " << count_number << std::endl;
    std::cout << "Warmup count numbers: " << warmup_count << std::endl;
    for (size_t i = 0; i < warmup_count; ++i) {
        Generator1 generator1(seed);
        for (size_t i = 0; i < count_number; ++i) {
            generator1();
        }
        Generator2 generator2(seed);
        for (size_t i = 0; i < count_number; ++i) {
            generator2();
        }
    }
    Generator1 generator1(seed);
    Generator2 generator2(seed);
    // std::vector<typename Generator1::result_type> numbers1(count_number);
    // std::vector<typename Generator2::result_type> numbers2(count_number);
    auto begin = std::chrono::steady_clock::now();
    for (size_t i = 0; i < count_number; ++i) {
        generator1();
    }
    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count();
    std::cout << "Generate with " << generator1_name << ": " << elapsed << " ms" << std::endl;

    begin = std::chrono::steady_clock::now();
    for (size_t i = 0; i < count_number; ++i) {
        generator2();
    }
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count();
    std::cout << "Generate with " << generator2_name << ": " << elapsed << " ms" << std::endl;
}
