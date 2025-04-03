#include <cassert>
#include <chrono>
#include <cpuid.h>
#include <iostream>

#include "generators/linear_congruential_generator.hpp"
#include "generators/mersenne_twister.hpp"
#include "generators/mersenne_twister_simd.hpp"
#include "statistical_test/nist.hpp"

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
    statistical_test::NistTest nist_test({bytes});
    nist_test.test(true);
}

#if defined(__AVX__) && defined(__AVX2__)
bool check_correct_avx2(size_t count_number) {
    MT19937 right_gen;
    MT32AVX2 gen;
    size_t count = 0;
    for (size_t i = 0; i < count_number; i++) {
        uint32_t right = right_gen();
        uint32_t value = gen();
        if (right != value) {
            std::cout << "i = " << i << " Different value " << right << " != " << value << std::endl;
            count++;
        }
    }
    std::cout << "Different numbers: " << count << std::endl;
    return count == 0;
}

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
bool check_correct_avx512(size_t count_number) {
    MT19937 right_gen;
    MT32AVX512 gen;
    size_t count = 0;
    for (size_t i = 0; i < count_number; i++) {
        uint32_t right = right_gen();
        uint32_t value = gen();
        if (right != value) {
            std::cout << "i = " << i << " Different value " << right << " != " << value << std::endl;
            count++;
        }
    }
    std::cout << "Different numbers: " << count << std::endl;
    return count == 0;
}

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

bool is_avx512_enabled() {
    unsigned int eax, ebx, ecx, edx;

    // Проверка AVX512F (бит 16 в EBX при leaf=7)
    __cpuid_count(7, 0, eax, ebx, ecx, edx);
    if (!(ebx & (1 << 16))) {
        return false; // AVX512F не поддерживается
    }

    // Проверка регистра XCR0 (ZMM-регистры разрешены)
    const uint64_t xcr0 = _xgetbv(0);
    constexpr uint64_t XCR0_ZMM_MASK = 0xE0; // Биты 7:5 (ZMM0-15, ZMM16-31)
    return (xcr0 & XCR0_ZMM_MASK) == XCR0_ZMM_MASK;
}

void mt_nist_test(size_t count_tests, size_t count_number, uint32_t start_seed = 0u) {
    std::vector<utils::seq_bytes> bytes(count_tests);
    for (size_t i = 0; i < count_tests; ++i) {
        MT19937 generator(start_seed + i);
        std::vector<uint32_t> numbers(count_number);
        for (size_t j = 0; j < count_number; ++j) {
            numbers[j] = generator();
        }
        bytes[i] = utils::convert_numbers_to_seq_bytes(numbers);
        assert(bytes[i].size() == count_number * 32);
    }
    statistical_test::NistTest nist_test(bytes);
    nist_test.test();
}

int main() {
    std::size_t count_number = 1'000'000'000;
    // std::size_t count_number = 1'000'000'111;
    // std::size_t count_number = 100'000;

#if defined(__AVX__) && defined(__AVX2__)
    // std::cout << "AVX2\n";
    // check_correct_avx2(count_number);
// benchmark_generate_avx2(count_number);
// benchmark_generate_array_avx2(count_number);
#endif

#ifdef __AVX512F__
    // std::cout << "AVX512\n";
    // check_correct_avx512(count_number);
// benchmark_generate_avx512(count_number);
// benchmark_generate_array_avx512(count_number);
#endif

#if defined(__AVX__) && defined(__AVX2__) && defined(__AVX512F__)
    // benchmark_generate_avx2_vs_avx512(count_number);
    // benchmark_generate_array_avx2_vs_avx512(count_number);
#endif
    mt_nist_test(100, 15000, 12345);
    return 0;
}
