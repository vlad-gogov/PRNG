#pragma once

#include <bitset>
#include <complex>
#include <iostream>
#include <limits>
#include <numbers>
#include <vector>

namespace utils {

using seq_bytes = std::vector<unsigned char>;

template <class UIntType>
seq_bytes convert_number_to_seq_bytes(const UIntType number) {
    static_assert(std::is_integral_v<UIntType> && std::is_unsigned_v<UIntType>);
    if (number == 0U) {
        return {0};
    }
    std::bitset<sizeof(UIntType) * 8U> bits = number;
    std::string number_bits = bits.to_string();
    size_t length_number = sizeof(UIntType) * 8U;
    while (!(number & (1U << (length_number - 1U)))) {
        --length_number;
    }
    seq_bytes result;
    result.reserve(length_number);
    size_t size = number_bits.size();
    for (size_t i = size - length_number; i < size; i++) {
        result.push_back(number_bits[i] == '1' ? 1 : 0);
    }
    return result;
}

template <class UIntType>
seq_bytes convert_numbers_to_seq_bytes(const std::vector<UIntType> &numbers) {
    static_assert(std::is_integral_v<UIntType> && std::is_unsigned_v<UIntType>);
    size_t length_number = sizeof(UIntType) * 8U;
    seq_bytes result;
    result.reserve(length_number * numbers.size());
    for (const auto &number : numbers) {
        seq_bytes temp = convert_number_to_seq_bytes(number);
        result.insert(result.end(), temp.begin(), temp.end());
    }
    return result;
}

size_t get_max_run(const seq_bytes &seq, size_t left_border = 0U, size_t right_border = 0U);

template <typename T>
std::vector<std::complex<double>> DFT(const std::vector<T> &x) {
    size_t size = x.size();
    std::vector<std::complex<double>> result(size);
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            double a = 2 * std::numbers::pi * j * i / size;
            result[i] += std::complex<double>(std::cos(a), std::sin(a)) * (double)x[j];
        }
    }
    return result;
}

seq_bytes read_bits_from_exponent(size_t count = 0);

} // namespace utils
