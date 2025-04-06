#pragma once

#include <bitset>
#include <complex>
#include <iostream>
#include <limits>
#include <numbers>
#include <omp.h>
#include <vector>

namespace utils {

using seq_bytes = std::vector<unsigned char>;

template <class UIntType>
seq_bytes convert_number_to_seq_bytes(const UIntType number) {
    static_assert(std::is_integral_v<UIntType> && std::is_unsigned_v<UIntType>);
    if (number == 0U) {
        return seq_bytes(sizeof(UIntType) * 8U, 0);
    }
    std::bitset<sizeof(UIntType) * 8U> bits = number;
    std::string number_bits = bits.to_string();
    size_t size = number_bits.size();
    seq_bytes result;
    result.reserve(size);
    for (size_t i = 0; i < size; ++i) {
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
std::vector<std::complex<std::double_t>> DFT(const std::vector<T> &x) {
    size_t size = x.size();
    const std::double_t two_pi_over_size = 2 * std::numbers::pi / size;
    std::vector<std::complex<std::double_t>> result(size);
#pragma omp parallel for
    for (size_t i = 0; i < size; ++i) {
        const std::double_t delta = two_pi_over_size * i;
        std::complex<std::double_t> sum(0.0, 0.0);
        for (size_t j = 0; j < size; ++j) {
            sum += std::polar<std::double_t>(1, delta * j) * (std::double_t)x[j];
        }
        result[i] = sum;
    }
    return result;
}

seq_bytes read_bits_from_exponent(size_t count = 0);

template <typename T>
std::vector<std::complex<std::double_t>> FFT(const std::vector<T> &x) {
    const size_t size = x.size();

    if ((size == 0) || (size & (size - 1)) != 0) {
        throw std::invalid_argument("FFT requires input size to be a power of two");
    }

    std::vector<std::complex<std::double_t>> result(size);
    for (size_t i = 0; i < size; ++i) {
        result[i] = static_cast<std::double_t>(x[i]);
    }

    // 1. Bit-reversal permutation
    size_t j = 0;
    for (size_t i = 0; i < size - 1; ++i) {
        if (i < j) {
            std::swap(result[i], result[j]);
        }
        size_t mask = size >> 1;
        while (j >= mask) {
            j -= mask;
            mask >>= 1;
        }
        j += mask;
    }

    // 2. The basic FFT algorithm
    const std::double_t pi = std::numbers::pi;
    const size_t log2_size = static_cast<size_t>(std::log2(size));
    for (size_t s = 1; s <= log2_size; ++s) {
        // Block size
        const size_t m = 1 << s;
        // Half block size
        const size_t m2 = m >> 1;
        // Base angle
        const std::double_t theta = 2 * pi / m;

        // Calculation of rotation multipliers (twiddle factors)
        std::vector<std::complex<std::double_t>> w(m2);
        for (size_t k = 0; k < m2; ++k) {
            w[k] = std::polar(1.0, theta * k);
        }

        // Block processing
        for (size_t k = 0; k < size; k += m) {
            for (size_t j = 0; j < m2; ++j) {
                const std::complex<std::double_t> even = result[k + j];
                const std::complex<std::double_t> odd = result[k + j + m2] * w[j];
                result[k + j] = even + odd;
                result[k + j + m2] = even - odd;
            }
        }
    }

    return result;
}

std::vector<std::vector<int>> matrix_from_bytes(const seq_bytes &bytes, int rows, int cols, int offset = 0);
int binary_matrix_rank(std::vector<std::vector<int>> matrix, int cols, int rows);
seq_bytes read_bytes_from_file(const std::string path, size_t count);
double chi_square(std::vector<double> trial_vector, std::vector<double> expected_vector, int degrees_of_freedom);
double p_value(int degrees_of_freedom, double chi_square);
double poissonian(int k, double lambda);
std::vector<double> doubles_from_bits(const seq_bytes &bytes, int num_doubles);
std::vector<double> random_doubles(int num_doubles);
double kstest(std::vector<double> p_values);
int kperm(const std::vector<int> &v);
uint bits_to_uint(const seq_bytes &bytes, uint offset);
std::vector<uint> bits_to_vector_uint(const seq_bytes &bytes, int size);
} // namespace utils
