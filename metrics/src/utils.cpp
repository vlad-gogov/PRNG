#include "utils.hpp"

#include <algorithm>
#include <boost/math/special_functions/gamma.hpp>
#include <filesystem>
#include <fstream>

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

    return boost::math::gamma_q((double)(degrees_of_freedom) / 2.0, chi_square / 2.0);
}

double utils::poissonian(int k, double lambda) {
    return std::pow(lambda, k) * std::exp(-lambda) / boost::math::factorial<double>(k);
    ;
}