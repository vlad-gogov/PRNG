#include <cassert>
#include <chrono>
#include <cpuid.h>
#include <iostream>

#include <random>

#include "generators/linear_congruential_generator.hpp"
#include "generators/mersenne_twister.hpp"
#include "generators/mersenne_twister_involution.hpp"
#include "generators/mersenne_twister_sbox.hpp"
#include "generators/mersenne_twister_simd.hpp"
#include "generators/mersenne_twister_siphash.hpp"
#include "indicators.hpp"
#include "statistical_test/diehard.hpp"
#include "statistical_test/nist.hpp"
#include "statistical_test/statistical_test.hpp"

#include "utils.hpp"

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

    const size_t count_number = 32768;
    const size_t count_tests = 1000;

    // ratio_one_zero<MINSTD_RAND0>(count_number, 23482349);
    // run_statistical_test<statistical_test::NistTest, MINSTD_RAND>("MINSTD_RAND", count_tests, count_number,
    // 23482349);
    // run_statistical_test<statistical_test::NistTest, MT19937_64>("MT19937_64", count_tests, count_number / 2, 5489u);
    run_statistical_test<statistical_test::NistTest, MT19937>("MT19937", count_tests, count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOX>("MT19937SBOX", count_tests, count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, std::minstd_rand0>("lib_minstd_rand0", count_tests,
    // count_number,
    //                                                                     23482349);
    // run_statistical_test<statistical_test::NistTest, LCG_GLIBC>("LCG_GLIBC", count_tests, count_number,
    // 23482349); run_statistical_test<statistical_test::NistTest, LCG_Numerical_Recipes>("LCG_Numerical_Recipes",
    // count_tests,
    //                                                                         count_number, 23482349);
    // run_statistical_test<statistical_test::NistTest, LCG_Borland>("LCG_Borland", count_tests, count_number,
    // 23482349); run_statistical_test<statistical_test::NistTest, LCG_ANSI_C>("LCG_ANSI_C", count_tests,
    // count_number, 23482349); run_statistical_test<statistical_test::NistTest,
    // MINSTD_RAND_IMPROVE>("MINSTD_RAND_IMPROVE", 1000, 32768, 23482349);
    // run_statistical_test<statistical_test::NistTest, MT19937>("MT19937", 1000, 32768, 12345);
    // run_statistical_test<statistical_test::DiehardTest, MT19937>("MT19937", 1000, 32768, 12345);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOX>("MT19937SBOX", 1000, 32768, 12345);
    // print_gen_value<MT19937SBOX>(10);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXEnd>("MT19937SBOXEnd", 1000, 32768, 12345);
    // print_gen_value<MT19937SBOXEnd>(10);
    // run_statistical_test<statistical_test::NistTest, MT19937SIPHASH>("MT19937SIPHASH", 1000, 32768, 12345);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution3>("MT19937Involution3", count_tests,
    //                                                                      count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution14>("MT19937Involution14", count_tests,
    //                                                                       count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution24>("MT19937Involution24", count_tests,
    //                                                                       count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution26>("MT19937Involution24", count_tests,
    //                                                                       count_number, 5489u);
    // run_benchmark<std::mt19937, MT19937>("std::mt19937", "MT19937", count_number * count_number, 12345, 5);
    // run_benchmark<MT19937, MT19937Involution3>("MT19937", "MT19937Involution3", count_number * count_number, 12345,
    // 5);
    return 0;
}
