#pragma once

#include <iostream>
#include <limits>
#include <vector>

namespace utils {

using seq_bytes = std::vector<bool>;

template <class UIntType>
seq_bytes convert_number_to_seq_bytes(const UIntType number) {
    static_assert(std::is_integral_v<UIntType> && std::is_unsigned_v<UIntType>);
    if (number == 0U) {
        return {0U};
    }
    size_t length_number = sizeof(UIntType) * 8U;
    while (!(number & (1U << (length_number - 1U)))) {
        --length_number;
    }
    seq_bytes result;
    result.reserve(length_number);
    for (size_t i = 0; i < length_number; ++i) {
        result.push_back(number & (1U << (length_number - i - 1U)));
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

} // namespace utils
