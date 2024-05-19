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

size_t get_max_run(const seq_bytes &seq, size_t left_border = 0U, size_t right_border = 0U);

std::vector<std::vector<int>> matrix_from_bytes(const utils::seq_bytes &bytes, int rows, int cols, int offset = 0);

int binary_matrix_rank(std::vector<std::vector<int>> matrix, int cols, int rows);

seq_bytes read_bytes_from_file(const std::string path, size_t count);

double chi_square(std::vector<double> trial_vector, std::vector<double> expected_vector, int degrees_of_freedom);

double p_value(int degrees_of_freedom, double chi_square);

double poissonian(int k, double lambda);

std::vector<double> doubles_from_bits(const utils::seq_bytes &bytes, int num_doubles);

std::vector<double> random_doubles(int num_doubles);

double kstest(std::vector<double> p_values);

int kperm(const std::vector<int> &v);

uint bits_to_uint(const utils::seq_bytes &bytes, uint offset);
std::vector<uint> bits_to_vector_uint(const utils::seq_bytes &bytes, int size);

} // namespace utils
