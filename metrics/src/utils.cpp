#include "utils.hpp"

#include <algorithm>

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

std::vector<std::vector<int>> matrix_from_bytes(const utils::seq_bytes & bytes, int rows, int cols, int offset = 0) {

    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols));
    for (size_t row = 0; row < rows; row++) {
        for (size_t col = 0; col < cols; col++) {
            matrix[row][col] = bytes[offset + row * cols + col];
        }
    }

    return matrix;
}

int binary_matrix_rank(std::vector<std::vector<int>> matrix, int cols, int rows) {
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
        }
        else {
            bool reduce = true;
            for (size_t i = row + 1; i < rows; i++) {
                if (matrix[i][row]){
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