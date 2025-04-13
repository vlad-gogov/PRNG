#include <cassert>
#include <chrono>
#include <cpuid.h>
#include <iostream>

#include "generators/linear_congruential_generator.hpp"
#include "generators/mersenne_twister.hpp"
#include "generators/mersenne_twister_involutio.hpp"
#include "generators/mersenne_twister_sbox.hpp"
#include "generators/mersenne_twister_simd.hpp"
#include "generators/mersenne_twister_siphash.hpp"
#include "indicators.hpp"
#include "statistical_test/nist.hpp"
#include "statistical_test/statistical_test.hpp"

void lcg_nist_test(size_t count_number) {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    LinearCongruentialGenerator<std::uint32_t, a, c, m> generator(23482349);
    std::vector<std::uint32_t> numbers(count_number);
    for (std::uint32_t i = 0; i < count_number; ++i) {
        numbers[i] = generator();
    }
    utils::seq_bytes bytes = utils::convert_numbers_to_seq_bytes(numbers);
    statistical_test::NistTest nist_test;
    nist_test.test(bytes, true);
}

#if defined(__AVX__) && defined(__AVX2__)
void benchmark_generate_avx2(size_t count_number) {
    MT19937 right_gen;
    auto begin = std::chrono::steady_clock::now();
    for (size_t i = 0; i < count_number; i++) {
        right_gen();
    }
    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count();
    std::cout << "Generate with MT19937: " << elapsed << " ms" << std::endl;

    MT32AVX2 gen;
    begin = std::chrono::steady_clock::now();
    for (size_t i = 0; i < count_number; i++) {
        gen();
    }
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count();
    std::cout << "Generate with MT19937AVX2: " << elapsed << " ms" << std::endl;
}

void benchmark_generate_array_avx2(size_t count_number) {
    MT19937 right_gen;
    std::vector<uint32_t> array_right(count_number);
    auto begin = std::chrono::steady_clock::now();
    for (size_t i = 0; i < count_number; i++) {
        array_right[i] = right_gen();
    }
    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count();
    std::cout << "Fill with MT19937: " << elapsed << " ms" << std::endl;

    MT32AVX2 gen;
    std::vector<uint32_t> array(count_number);
    begin = std::chrono::steady_clock::now();
    gen.generate_bulk(array.data(), array.size());
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count();
    std::cout << "Fill with MT19937AVX2: " << elapsed << " ms" << std::endl;

    for (size_t i = 0; i < count_number; i++) {
        if (array_right[i] != array[i]) {
            std::cout << "i = " << i << " Diff value " << array_right[i] << " != " << array[i] << std::endl;
        }
    }
}
#endif

#ifdef __AVX512F__
void benchmark_generate_avx512(size_t count_number) {
    MT19937 right_gen;
    auto begin = std::chrono::steady_clock::now();
    for (size_t i = 0; i < count_number; i++) {
        right_gen();
    }
    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count();
    std::cout << "Generate with MT19937: " << elapsed << " ms" << std::endl;

    MT32AVX512 gen;
    begin = std::chrono::steady_clock::now();
    for (size_t i = 0; i < count_number; i++) {
        gen();
    }
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count();
    std::cout << "Generate with MT19937AVX512: " << elapsed << " ms" << std::endl;
}

void benchmark_generate_array_avx512(size_t count_number) {
    MT19937 right_gen;
    std::vector<uint32_t> array_right(count_number);
    auto begin = std::chrono::steady_clock::now();
    for (size_t i = 0; i < count_number; i++) {
        array_right[i] = right_gen();
    }
    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count();
    std::cout << "Fill with MT19937: " << elapsed << " ms" << std::endl;

    MT32AVX512 gen;
    std::vector<uint32_t> array(count_number);
    begin = std::chrono::steady_clock::now();
    gen.generate_bulk(array.data(), array.size());
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count();
    std::cout << "Fill with MT19937AVX512: " << elapsed << " ms" << std::endl;

    for (size_t i = 0; i < count_number; i++) {
        if (array_right[i] != array[i]) {
            std::cout << "i = " << i << " Diff value " << array_right[i] << " != " << array[i] << std::endl;
        }
    }
}
#endif

#if defined(__AVX__) && defined(__AVX2__) && defined(__AVX512F__)
void benchmark_generate_avx2_vs_avx512(size_t count_number) {
    MT32AVX2 right_gen;
    auto begin = std::chrono::steady_clock::now();
    for (size_t i = 0; i < count_number; i++) {
        right_gen();
    }
    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count();
    std::cout << "Generate with MT19937AVX2: " << elapsed << " ms" << std::endl;

    MT32AVX512 gen;
    begin = std::chrono::steady_clock::now();
    for (size_t i = 0; i < count_number; i++) {
        gen();
    }
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count();
    std::cout << "Generate with MT19937AVX512: " << elapsed << " ms" << std::endl;
}

void benchmark_generate_array_avx2_vs_avx512(size_t count_number) {
    MT32AVX2 right_gen;
    std::vector<uint32_t> array_right(count_number);
    auto begin = std::chrono::steady_clock::now();
    right_gen.generate_bulk(array_right.data(), array_right.size());
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    std::cout << "Fill with MT19937AVX2: " << elapsed << " ms" << std::endl;

    MT32AVX512 gen;
    std::vector<uint32_t> array(count_number);
    begin = std::chrono::steady_clock::now();
    gen.generate_bulk(array.data(), array.size());
    end = std::chrono::steady_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    std::cout << "Fill with MT19937AVX512: " << elapsed << " ms" << std::endl;

    for (size_t i = 0; i < count_number; i++) {
        if (array_right[i] != array[i]) {
            std::cout << "i = " << i << " Diff value " << array_right[i] << " != " << array[i] << std::endl;
        }
    }
}
#endif

template <typename StatisticalTest, typename Generator>
void run_test(const std::string &generator_name, const size_t count_tests, const size_t count_number,
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
        std::vector<typename Generator::ValueType> numbers(count_number);
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
void print_gen_value(const size_t count_numbers, const uint32_t seed = 0u) {
    Generator generator(seed);
    for (size_t i = 0; i < count_numbers; ++i) {
        std::cout << generator() << " ";
    }
    std::cout << std::endl;
}

int main() {
    // std::size_t count_number = 1'000'000'000;

#if defined(__AVX__) && defined(__AVX2__)
    // std::cout << "AVX2\n";
    // benchmark_generate_avx2(count_number);
    // benchmark_generate_array_avx2(count_number);
#endif

#ifdef __AVX512F__
    // std::cout << "AVX512\n";
    // benchmark_generate_avx512(count_number);
    // benchmark_generate_array_avx512(count_number);
#endif

#if defined(__AVX__) && defined(__AVX2__) && defined(__AVX512F__)
    // benchmark_generate_avx2_vs_avx512(count_number);
    // benchmark_generate_array_avx2_vs_avx512(count_number);
#endif

    // run_test<statistical_test::NistTest, MT19937>("MT19937", 100, 16384, 12345);
    // run_test<statistical_test::NistTest, MT19937SBOX>("MT19937SBOX", 100, 16384, 12345);
    // print_gen_value<MT19937SBOX>(10);
    // run_test<statistical_test::NistTest, MT19937SBOXEnd>("MT19937SBOXEnd", 100, 16384, 12345);
    // print_gen_value<MT19937SBOXEnd>(10);
    // run_test<statistical_test::NistTest, MT19937SIPHASH>("MT19937SIPHASH", 100, 16384, 12345);
    // run_test<statistical_test::NistTest, MT19937Involution3>("MT19937Involution3", 100, 16384, 12345);
    return 0;
}
