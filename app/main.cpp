#include <cassert>
#include <chrono>
#include <cpuid.h>
#include <iostream>

#include <random>

#include "generators/linear_congruential_generator.hpp"
#include "generators/mersenne_twister.hpp"
#include "generators/mersenne_twister_rotr.hpp"
#include "generators/mersenne_twister_sbox.hpp"
#include "generators/mersenne_twister_sbox_and_rotr.hpp"
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

void benchmark_opt_sbox_and_rotr(size_t count_number) {
    MT19937SBOXRotr31 right_gen;
    auto begin = std::chrono::steady_clock::now();
    for (size_t i = 0; i < count_number; i++) {
        right_gen();
    }
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    std::cout << "Get with MT19937AVX2: " << elapsed << " ms" << std::endl;

    MT32SboxRotr31AVX2 gen;
    begin = std::chrono::steady_clock::now();
    for (size_t i = 0; i < count_number; i++) {
        gen();
    }
    end = std::chrono::steady_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    std::cout << "Get with MT32SboxRotr31AVX2: " << elapsed << " ms" << std::endl;
}

void benchmark_opt_sbox_and_rotr_generate(size_t count_number) {
    MT19937SBOXRotr31 right_gen;
    std::vector<uint32_t> array_right(count_number);
    auto begin = std::chrono::steady_clock::now();
    for (size_t i = 0; i < count_number; i++) {
        array_right[i] = right_gen();
    }
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    std::cout << "Fill with MT19937AVX2: " << elapsed << " ms" << std::endl;

    {
        MT32SboxRotr31AVX2 gen;
        std::vector<uint32_t> array(count_number);
        begin = std::chrono::steady_clock::now();
        for (size_t i = 0; i < count_number; i++) {
            array[i] = gen();
        }
        end = std::chrono::steady_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        std::cout << "Fill with MT32SboxRotr31AVX2: " << elapsed << " ms" << std::endl;
    }

    MT32SboxRotr31AVX2 gen;
    std::vector<uint32_t> array(count_number);
    begin = std::chrono::steady_clock::now();
    gen.generate_bulk(array.data(), array.size());
    end = std::chrono::steady_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    std::cout << "Bulk with MT32SboxRotr31AVX2: " << elapsed << " ms" << std::endl;

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
    std::size_t count_number = 100'000'000;

#if defined(__AVX__) && defined(__AVX2__)
    // std::cout << "AVX2\n";
    // benchmark_generate_avx2(count_number);
    // benchmark_generate_array_avx2(count_number);
    benchmark_opt_sbox_and_rotr_generate(count_number);
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

    // const size_t count_number = 32768;
    // const size_t count_tests = 1000;
    // const std::double_t alpha = 1.0 / count_tests;

    // ratio_one_zero<MINSTD_RAND0>(count_number, 23482349);
    // run_statistical_test<statistical_test::NistTest, MINSTD_RAND>("MINSTD_RAND", count_tests, count_number,
    // 23482349,
    //                                                               alpha);
    // run_statistical_test<statistical_test::NistTest, MINSTD_RAND0>("MINSTD_RAND0", count_tests, count_number,
    // 23482349,
    //                                                                alpha);

    // run_statistical_test<statistical_test::NistTest, std::minstd_rand0>("lib_minstd_rand0", count_tests,
    // count_number,
    //                                                                     23482349, alpha);
    // run_statistical_test<statistical_test::NistTest, LCG_GLIBC>("LCG_GLIBC", count_tests, count_number, 23482349,
    //                                                             alpha);
    // run_statistical_test<statistical_test::NistTest, LCG_Numerical_Recipes>("LCG_Numerical_Recipes", count_tests,
    //                                                                         count_number, 23482349, alpha);
    // run_statistical_test<statistical_test::NistTest, LCG_Borland>("LCG_Borland", count_tests, count_number,
    // 23482349,
    //                                                               alpha);
    // run_statistical_test<statistical_test::NistTest, LCG_ANSI_C>("LCG_ANSI_C", count_tests, count_number,
    // 23482349,
    //                                                              alpha);

    // run_statistical_test<statistical_test::NistTest, MT19937>("MT19937", count_tests, count_number, 5489u,
    // alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937_64>("MT19937_64", count_tests, count_number / 2, 5489u,
    //                                                              alpha);

    // run_statistical_test<statistical_test::NistTest, MT19937Rotr1>("MT19937Rotr1", count_tests,
    //                                                                      count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937Rotr2>("MT19937Rotr2", count_tests,
    //                                                                      count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937Rotr3>("MT19937Rotr3", count_tests,
    //                                                                      count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937Rotr4>("MT19937Rotr4", count_tests,
    //                                                                      count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937Rotr5>("MT19937Rotr5", count_tests,
    //                                                                      count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937Rotr6>("MT19937Rotr6", count_tests,
    //                                                                      count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937Rotr7>("MT19937Rotr7", count_tests,
    //                                                                      count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937Rotr8>("MT19937Rotr8", count_tests,
    //                                                                      count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937Rotr9>("MT19937Rotr9", count_tests,
    //                                                                      count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937Rotr10>("MT19937Rotr10", count_tests,
    //                                                                       count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937Rotr11>("MT19937Rotr11", count_tests,
    //                                                                       count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937Rotr12>("MT19937Rotr12", count_tests,
    //                                                                       count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937Rotr13>("MT19937Rotr13", count_tests,
    //                                                                       count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937Rotr14>("MT19937Rotr14", count_tests,
    //                                                                       count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937Rotr15>("MT19937Rotr15", count_tests,
    //                                                                       count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937Rotr16>("MT19937Rotr16", count_tests,
    //                                                                       count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937Rotr17>("MT19937Rotr17", count_tests,
    //                                                                       count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937Rotr18>("MT19937Rotr18", count_tests,
    //                                                                       count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937Rotr19>("MT19937Rotr19", count_tests,
    //                                                                       count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937Rotr20>("MT19937Rotr20", count_tests,
    //                                                                       count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937Rotr21>("MT19937Rotr21", count_tests,
    //                                                                       count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937Rotr22>("MT19937Rotr22", count_tests,
    //                                                                       count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937Rotr23>("MT19937Rotr23", count_tests,
    //                                                                       count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937Rotr24>("MT19937Rotr24", count_tests,
    //                                                                       count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937Rotr25>("MT19937Rotr25", count_tests,
    //                                                                       count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937Rotr26>("MT19937Rotr26", count_tests,
    //                                                                       count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937Rotr27>("MT19937Rotr27", count_tests,
    //                                                                       count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937Rotr28>("MT19937Rotr28", count_tests,
    //                                                                       count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937Rotr29>("MT19937Rotr29", count_tests,
    //                                                                       count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937Rotr30>("MT19937Rotr30", count_tests,
    //                                                                       count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937Rotr31>("MT19937Rotr31", count_tests,
    //                                                                       count_number, 5489u, alpha);

    // run_statistical_test<statistical_test::NistTest, MT19937SBOX>("MT19937SBOX", count_tests, count_number, 5489u,
    //                                                               alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXEnd>("MT19937SBOXEnd", count_tests, count_number,
    // 5489u,
    //                                                                  alpha);

    // run_statistical_test<statistical_test::NistTest, MT19937SBOXRotr1>("MT19937SBOXRotr1",
    // count_tests,
    //                                                                          count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXRotr2>("MT19937SBOXRotr2",
    // count_tests,
    //                                                                          count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXRotr3>("MT19937SBOXRotr3",
    // count_tests,
    //                                                                          count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXRotr4>("MT19937SBOXRotr4",
    // count_tests,
    //                                                                          count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXRotr5>("MT19937SBOXRotr5",
    // count_tests,
    //                                                                          count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXRotr6>("MT19937SBOXRotr6",
    // count_tests,
    //                                                                          count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXRotr7>("MT19937SBOXRotr7",
    // count_tests,
    //                                                                          count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXRotr8>("MT19937SBOXRotr8",
    // count_tests,
    //                                                                          count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXRotr9>("MT19937SBOXRotr9",
    // count_tests,
    //                                                                          count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXRotr10>("MT19937SBOXRotr10",
    // count_tests,
    //                                                                           count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXRotr11>("MT19937SBOXRotr11",
    // count_tests,
    //                                                                           count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXRotr12>("MT19937SBOXRotr12",
    // count_tests,
    //                                                                           count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXRotr13>("MT19937SBOXRotr13",
    // count_tests,
    //                                                                           count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXRotr14>("MT19937SBOXRotr14",
    // count_tests,
    //                                                                           count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXRotr15>("MT19937SBOXRotr15",
    // count_tests,
    //                                                                           count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXRotr16>("MT19937SBOXRotr16",
    // count_tests,
    //                                                                           count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXRotr17>("MT19937SBOXRotr17",
    // count_tests,
    //                                                                           count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXRotr18>("MT19937SBOXRotr18",
    // count_tests,
    //                                                                           count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXRotr19>("MT19937SBOXRotr19",
    // count_tests,
    //                                                                           count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXRotr20>("MT19937SBOXRotr20",
    // count_tests,
    //                                                                           count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXRotr21>("MT19937SBOXRotr21",
    // count_tests,
    //                                                                           count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXRotr22>("MT19937SBOXRotr22",
    // count_tests,
    //                                                                           count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXRotr23>("MT19937SBOXRotr23",
    // count_tests,
    //                                                                           count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXRotr24>("MT19937SBOXRotr24",
    // count_tests,
    //                                                                           count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXRotr25>("MT19937SBOXRotr25",
    // count_tests,
    //                                                                           count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXRotr26>("MT19937SBOXRotr26",
    // count_tests,
    //                                                                           count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXRotr27>("MT19937SBOXRotr27",
    // count_tests,
    //                                                                           count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXRotr28>("MT19937SBOXRotr28",
    // count_tests,
    //                                                                           count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXRotr29>("MT19937SBOXRotr29",
    // count_tests,
    //                                                                           count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXRotr30>("MT19937SBOXRotr30",
    // count_tests,
    //                                                                           count_number, 5489u, alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXRotr31>("MT19937SBOXRotr31",
    // count_tests,
    //                                                                           count_number, 5489u, alpha);

    // run_statistical_test<statistical_test::NistTest, MT19937SIPHASH>("MT19937SIPHASH", count_tests, count_number,
    // 5489u,
    //                                                                  alpha);
    // run_statistical_test<statistical_test::NistTest, MT19937SIPHASH_64>("MT19937SIPHASH_64", count_tests,
    //                                                                     count_number / 2, 5489u, alpha);
    // 15 +
    // run_benchmark<MT19937, MT19937SBOXRotr31>("MT19937", "MT19937SBOXRotr31", count_number * 2048, 5489u, 5);
    // run_benchmark<MT19937, MT19937SBOXRotr31>("MT19937", "MT19937SBOXRotr31", count_number * 4096, 5489u, 2);
    // run_benchmark<MT19937, MT19937SBOXRotr31>("MT19937", "MT19937SBOXRotr31", count_number * 8192, 5489u, 2);
    // run_benchmark<MT19937, MT19937SBOXRotr31>("MT19937", "MT19937SBOXRotr31", count_number * 16384, 5489u, 2);
    // run_benchmark<MT19937, MT19937SBOXRotr31>("MT19937", "MT19937SBOXRotr31", count_number * 32768, 5489u, 2);
    // run_benchmark<MT19937, MT19937SBOXRotr31>("MT19937", "MT19937SBOXRotr31", count_number * 65536, 5489u, 2);
    // run_benchmark<MT19937, MT19937SBOXRotr31>("MT19937", "MT19937SBOXRotr31", count_number * 131072, 5489u, 2);
    // run_benchmark<MT19937, MT19937Rotr3>("MT19937", "MT19937Rotr3", count_number * count_number,
    // 5489u,
    // 5);
    // print_gen_value<MT19937SBOX>(10);
    return 0;
}
