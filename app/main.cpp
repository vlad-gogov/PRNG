#include <chrono>
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
    statistical_test::NistTest nist_test(bytes);
    nist_test.test(true);
}

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
    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count();
    std::cout << "Fill with MT19937AVX2: " << elapsed << " ms" << std::endl;

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

int main() {
    // std::size_t count_number = 1'000'000'000;
    // std::size_t count_number = 1'000'000'111;
    // std::size_t count_number = 100'000;

    // AVX2
    // check_correct_avx2(count_number);
    // benchmark_generate_avx2(count_number);
    // benchmark_generate_array_avx2(count_number);

    // AVX512
    // check_correct_avx512(count_number);
    // benchmark_generate_avx512(count_number);
    // benchmark_generate_array_avx512(count_number);

    // AVX vs AVX512
    // benchmark_generate_avx2_vs_avx512(count_number);
    // benchmark_generate_array_avx2_vs_avx512(count_number);
    return 0;
}
