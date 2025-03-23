#include <iostream>

#include "generators/linear_congruential_generator.hpp"
#include "statistical_test/nist.hpp"

int main() {
    constexpr std::uint32_t a = 16807U;
    constexpr std::uint32_t c = 0U;
    constexpr std::uint32_t m = std::numeric_limits<std::uint32_t>::max();
    LinearCongruentialGenerator<std::uint32_t, a, c, m> generator(23482349);
    std::uint32_t count_number = 50;
    std::vector<std::uint32_t> numbers(count_number);
    for (std::uint32_t i = 0; i < count_number; ++i) {
        numbers[i] = generator();
    }
    utils::seq_bytes bytes = utils::convert_numbers_to_seq_bytes(numbers);
    statistical_test::NistTest nist_test(bytes);
    nist_test.test(true);
    return 0;
}
