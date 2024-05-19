#include "utils.hpp"

#include <algorithm>
#include <bitset>
#include <boost/math/special_functions/gamma.hpp>
#include <filesystem>
#include <fstream>
#include <limits>

size_t utils::get_max_run(const seq_bytes &seq, size_t left_border, size_t right_border) {
    if (right_border == 0U) {
        right_border = seq.size();
    }
    size_t max_run = 0U;
    size_t prev_index = left_border;
    bool isSeq = false;
    for (size_t i = left_border; i < right_border; ++i) {
        bool curr = seq[i];
        if (!isSeq && curr) {
            prev_index = i;
            isSeq = true;
        } else if (!curr && isSeq) {
            max_run = std::max(max_run, i - prev_index);
            isSeq = false;
        }
    }
    if (isSeq) {
        max_run = std::max(max_run, right_border - prev_index);
    }
    return max_run;
}

std::vector<std::vector<int>> utils::matrix_from_bytes(const utils::seq_bytes &bytes, int rows, int cols, int offset) {

    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols));
    for (size_t row = 0; row < rows; row++) {
        for (size_t col = 0; col < cols; col++) {
            matrix[row][col] = bytes[offset + row * cols + col];
        }
    }

    return matrix;
}

int utils::binary_matrix_rank(std::vector<std::vector<int>> matrix, int cols, int rows) {
    int rank = cols;
    for (size_t row = 0; row < rank; row++) {
        if (matrix[row][row]) {
            for (size_t col = 0; col < rows; col++) {
                if (col != row) {
                    for (size_t i = 0; i < rank; i++) {
                        matrix[col][i] ^= matrix[row][i];
                    }
                }
            }
        } else {
            bool reduce = true;
            for (size_t i = row + 1; i < rows; i++) {
                if (matrix[i][row]) {
                    std::swap(matrix[row], matrix[i]);
                    reduce = false;
                    break;
                }
            }
            if (reduce) {
                rank--;
                for (size_t i = 0; i < rows; i++) {
                    matrix[i][row] = matrix[i][rank];
                }
            }
            row--;
        }
    }
    return rank;
}

utils::seq_bytes utils::read_bytes_from_file(const std::string path, size_t count) {
    utils::seq_bytes bytes(count);
    std::stringstream path_stream;
    path_stream << std::string(std::filesystem::current_path().parent_path().c_str());
    path_stream << "/../";
    path_stream << path;
    std::ifstream e_file(path_stream.str());
    size_t index = 0;
    while (e_file.is_open() && index < count) {
        char sym = e_file.get();
        if (sym == '1') {
            bytes[index++] = 1;
        } else if (sym == '0') {
            bytes[index++] = 0;
        }
    }
    return bytes;
}

double utils::chi_square(std::vector<double> trial_vector, std::vector<double> expected_vector,
                         int degrees_of_freedom) {
    double chi_sq = 0.0;
    for (size_t i = 0; i < degrees_of_freedom; i++) {
        double part_chi_sq =
            (trial_vector[i] - expected_vector[i]) * (trial_vector[i] - expected_vector[i]) / expected_vector[i];
        chi_sq += part_chi_sq;
    }
    return chi_sq;
}

double utils::p_value(int degrees_of_freedom, double chi_square) {
    // std::cout << "degrees_of_freedom = " << degrees_of_freedom << std::endl;
    // std::cout << "chi_square = " << chi_square << std::endl;
    return boost::math::gamma_q((double)(degrees_of_freedom) / 2.0, chi_square / 2.0);
}

double utils::poissonian(int k, double lambda) {
    return std::pow(lambda, k) * std::exp(-lambda) / boost::math::factorial<double>(k);
}

std::vector<double> utils::doubles_from_bits(const utils::seq_bytes &bytes, int num_doubles) {
    // Converts seq_bytes to doubles within [0, 1]
    std::vector<double> result_doubles(num_doubles);
    for (size_t i = 0; i < num_doubles; i++) {
        std::bitset<64> num_bitset;
        for (size_t j = 0; j < 64; j++) {
            num_bitset[j] = bytes[i * 64 + j];
        }
        unsigned long long c = num_bitset.to_ullong();
        double val = *reinterpret_cast<double *>(&c);
        // std::cout << val / std::numeric_limits<double>::max() << "; ";
        result_doubles.push_back(val / std::numeric_limits<double>::max());
        // result_doubles.push_back(val / (std::pow(2, 32) - 1));
    }
    return result_doubles;
}

std::vector<double> utils::random_doubles(int num_doubles) {
    std::vector<double> result_doubles(num_doubles);
    for (size_t i = 0; i < num_doubles; i++) {
        double x = ((double)std::rand() / (RAND_MAX));
        result_doubles.push_back(x);
    }
    return result_doubles;
}

double utils::kstest(std::vector<double> p_values) {
    // Calculates p_value for vector of p_values.
    int count = p_values.size();
    // for (size_t i = 0; i < count; i++) {
    //     std::cout << p_values[i] << ", ";
    // }
    // std::cout << std::endl;

    if (count < 1)
        return -1;
    if (count == 1)
        return p_values[0];
    double d_max = 0.0;
    for (size_t i = 0; i < count; i++) {
        double y = (double)i / (count + 1.0);
        double d1 = p_values[i - 1] - y;
        double d2 = std::abs(1.0 / (count + 1.0) - d1);
        d1 = std::abs(d1);
        double d = std::max(d1, d2);
        if (d > d_max)
            d_max = d;
    }
    // std::cout << "D_max = " << d_max << std::endl;
    double s = d_max * d_max * count;
    return 2.0 * std::exp(-(2.000071 + .331 / std::sqrt(count) + 1.409 / count) * s);
}

int utils::kperm(const std::vector<int> &v) {
    // This function computes a unique permutation index for a given array of five integers.
    // This index uniquely represents the ordering of the integers in a factoradic numbering system, effectively mapping
    // the permutation to a specific number.
    std::vector<int> w = v;
    int pindex = 0, uret, tmp;

    // Work on a copy of v, not v itself in case we are using overlapping 5-patterns.
    for (int i = 4; i > 0; --i) {
        int max = w[0];
        int k = 0;
        for (int j = 1; j <= i; ++j) {
            if (max <= w[j]) {
                max = w[j];
                k = j;
            }
        }
        pindex = (i + 1) * pindex + k;
        std::swap(w[i], w[k]);
    }

    uret = pindex;

    return uret;
}

uint utils::bits_to_uint(const utils::seq_bytes &bytes, uint offset) {
    std::bitset<31> bitset;
    for (size_t i = 0; i < 31; i++) {
        bitset[i] = bytes[offset + i];
    }
    return bitset.to_ulong();
}

std::vector<uint> utils::bits_to_vector_uint(const utils::seq_bytes &bytes, int size) {
    std::vector<uint> result(size);
    for (size_t i = 0; i < size; i++) {
        result[i] = bits_to_uint(bytes, 31 * i);
    }
    return result;
}