#include <chrono>
#include <iostream>

#include "generators/linear_congruential_generator.hpp"
#include "generators/mersenne_twister.hpp"
#include "generators/mersenne_twister_simd.hpp"
#include "statistical_test/nist.hpp"

int main() {
    std::size_t count_number = 1'000'000'000;

    // constexpr std::uint32_t a = 16807U;
    // constexpr std::uint32_t c = 0U;
    // constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    // LinearCongruentialGenerator<std::uint32_t, a, c, m> generator(23482349);
    // std::vector<std::uint32_t> numbers(count_number);
    // for (std::uint32_t i = 0; i < count_number; ++i) {
    //     numbers[i] = generator();
    // }
    // utils::seq_bytes bytes = utils::convert_numbers_to_seq_bytes(numbers);
    // statistical_test::NistTest nist_test(bytes);
    // nist_test.test(true);

    // MT19937 right_gen;
    // auto begin = std::chrono::steady_clock::now();
    // for (size_t i = 0; i < count_number; i++) {
    //     right_gen();
    // }
    // auto elapsed =
    //     std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count();
    // std::cout << "MT19937: " << elapsed << "ms" << std::endl;

    // MersenneTwister32AVX2 gen;
    // begin = std::chrono::steady_clock::now();
    // for (size_t i = 0; i < count_number; i++) {
    //     gen();
    // }
    // elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() -
    // begin).count(); std::cout << "MT19937: " << elapsed << "ms" << std::endl; return 0;

    MT19937 right_gen;
    std::vector<uint32_t> array(count_number);
    auto begin = std::chrono::steady_clock::now();
    for (size_t i = 0; i < count_number; i++) {
        array[i] = right_gen();
    }
    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count();
    std::cout << "Fill with MT19937: " << elapsed << "ms" << std::endl;

    MersenneTwister32AVX2 gen;
    begin = std::chrono::steady_clock::now();
    gen.generate_bulk(array.data(), array.size());
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count();
    std::cout << "Fill with MT19937AVX2: " << elapsed << "ms" << std::endl;
    return 0;
}
