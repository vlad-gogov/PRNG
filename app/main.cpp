#include <cassert>
#include <chrono>
#include <cpuid.h>
#include <iostream>

#include <random>

#include "generators/linear_congruential_generator.hpp"
#include "generators/mersenne_twister.hpp"
#include "generators/mersenne_twister_involution.hpp"
#include "generators/mersenne_twister_sbox.hpp"
#include "generators/mersenne_twister_sbox_and_involution.hpp"
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
    // run_statistical_test_without_progress_bar<statistical_test::NistTest, MINSTD_RAND>("MINSTD_RAND", count_tests,
    //                                                                                    count_number, 23482349);
    // run_statistical_test_without_progress_bar<statistical_test::NistTest, MINSTD_RAND0>("MINSTD_RAND0", count_tests,
    //                                                                                     count_number, 23482349);
    // run_statistical_test_without_progress_bar<statistical_test::NistTest, MT19937>("MT19937", count_tests,
    // count_number,
    //                                                                                5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937_64>("MT19937_64", count_tests, count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOX>("MT19937SBOX", count_tests, count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXEnd>("MT19937SBOXEnd", count_tests, count_number,
    //                                                                  5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution1>("MT19937Involution1", count_tests,
    //                                                                      count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution2>("MT19937Involution2", count_tests,
    //                                                                      count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution3>("MT19937Involution3", count_tests,
    //                                                                      count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution4>("MT19937Involution4", count_tests,
    //                                                                      count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution5>("MT19937Involution5", count_tests,
    //                                                                      count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution6>("MT19937Involution6", count_tests,
    //                                                                      count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution7>("MT19937Involution7", count_tests,
    //                                                                      count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution8>("MT19937Involution8", count_tests,
    //                                                                      count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution9>("MT19937Involution9", count_tests,
    //                                                                      count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution10>("MT19937Involution10", count_tests,
    //                                                                       count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution11>("MT19937Involution11", count_tests,
    //                                                                       count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution12>("MT19937Involution12", count_tests,
    //                                                                       count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution13>("MT19937Involution13", count_tests,
    //                                                                       count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution14>("MT19937Involution14", count_tests,
    //                                                                       count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution15>("MT19937Involution15", count_tests,
    //                                                                       count_number, 5489u);
    run_statistical_test<statistical_test::NistTest, MT19937Involution16>("MT19937Involution16", count_tests,
                                                                          count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution17>("MT19937Involution17", count_tests,
    //                                                                       count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution18>("MT19937Involution18", count_tests,
    //                                                                       count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution19>("MT19937Involution19", count_tests,
    //                                                                       count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution20>("MT19937Involution20", count_tests,
    //                                                                       count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution21>("MT19937Involution21", count_tests,
    //                                                                       count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution22>("MT19937Involution22", count_tests,
    //                                                                       count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution23>("MT19937Involution23", count_tests,
    //                                                                       count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution24>("MT19937Involution24", count_tests,
    //                                                                       count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution25>("MT19937Involution25", count_tests,
    //                                                                       count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution26>("MT19937Involution26", count_tests,
    //                                                                       count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution27>("MT19937Involution27", count_tests,
    //                                                                       count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution28>("MT19937Involution28", count_tests,
    //                                                                       count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution29>("MT19937Involution29", count_tests,
    //                                                                       count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution30>("MT19937Involution30", count_tests,
    //                                                                       count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937Involution31>("MT19937Involution31", count_tests,
    //                                                                       count_number, 5489u);
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

    // run_statistical_test<statistical_test::NistTest, MT19937SBOXInvolution1>("MT19937SBOXInvolution1", count_tests,
    //                                                                          count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXInvolution2>("MT19937SBOXInvolution2", count_tests,
    //                                                                          count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXInvolution3>("MT19937SBOXInvolution3", count_tests,
    //                                                                          count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXInvolution4>("MT19937SBOXInvolution4", count_tests,
    //                                                                          count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXInvolution5>("MT19937SBOXInvolution5", count_tests,
    //                                                                          count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXInvolution6>("MT19937SBOXInvolution6", count_tests,
    //                                                                          count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXInvolution7>("MT19937SBOXInvolution7", count_tests,
    //                                                                          count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXInvolution8>("MT19937SBOXInvolution8", count_tests,
    //                                                                          count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXInvolution9>("MT19937SBOXInvolution9", count_tests,
    //                                                                          count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXInvolution10>("MT19937SBOXInvolution10", count_tests,
    //                                                                           count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXInvolution11>("MT19937SBOXInvolution11", count_tests,
    //                                                                           count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXInvolution12>("MT19937SBOXInvolution12", count_tests,
    //                                                                           count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXInvolution13>("MT19937SBOXInvolution13", count_tests,
    //                                                                           count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXInvolution14>("MT19937SBOXInvolution14", count_tests,
    //                                                                           count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXInvolution15>("MT19937SBOXInvolution15", count_tests,
    //                                                                           count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXInvolution16>("MT19937SBOXInvolution16", count_tests,
    //                                                                           count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXInvolution17>("MT19937SBOXInvolution17", count_tests,
    //                                                                           count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXInvolution18>("MT19937SBOXInvolution18", count_tests,
    //                                                                           count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXInvolution19>("MT19937SBOXInvolution19", count_tests,
    //                                                                           count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXInvolution20>("MT19937SBOXInvolution20", count_tests,
    //                                                                           count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXInvolution21>("MT19937SBOXInvolution21", count_tests,
    //                                                                           count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXInvolution22>("MT19937SBOXInvolution22", count_tests,
    //                                                                           count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXInvolution23>("MT19937SBOXInvolution23", count_tests,
    //                                                                           count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXInvolution24>("MT19937SBOXInvolution24", count_tests,
    //                                                                           count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXInvolution25>("MT19937SBOXInvolution25", count_tests,
    //                                                                           count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXInvolution26>("MT19937SBOXInvolution26", count_tests,
    //                                                                           count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXInvolution27>("MT19937SBOXInvolution27", count_tests,
    //                                                                           count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXInvolution28>("MT19937SBOXInvolution28", count_tests,
    //                                                                           count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXInvolution29>("MT19937SBOXInvolution29", count_tests,
    //                                                                           count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXInvolution30>("MT19937SBOXInvolution30", count_tests,
    //                                                                           count_number, 5489u);
    // run_statistical_test<statistical_test::NistTest, MT19937SBOXInvolution31>("MT19937SBOXInvolution31", count_tests,
    //                                                                           count_number, 5489u);
    return 0;
}
