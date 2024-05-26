#include "metrics/binary_matrix.hpp"

BinaryMatrix::BinaryMatrix() : size(0) {
}

BinaryMatrix::BinaryMatrix(const utils::seq_bytes bytes, size_t size) : bytes(bytes), size(size) {
}

enum class MatrixElimination {
    FORWARD,
    BACKWARD,
};

void elementary_row_operations(utils::seq_bytes &bytes, size_t size, int row, MatrixElimination matrix_elimination) {
    if (matrix_elimination == MatrixElimination::FORWARD) {
        for (int i = row + 1; i < size; i++) {
            if (bytes[i * size + row] == 1) {
                for (int j = row; j < size; j++) {
                    bytes[i * size + j] = (bytes[i * size + j] + bytes[row * size + j]) % 2;
                }
            }
        }
    } else {
        for (int i = row - 1; i >= 0; i--) {
            if (bytes[i * size + row] == 1) {
                for (int j = 0; j < size; j++) {
                    bytes[i * size + j] = (bytes[i * size + j] + bytes[row * size + j]) % 2;
                }
            }
        }
    }
}

void swap_rows(utils::seq_bytes &bytes, size_t size, size_t first, size_t second) {
    for (size_t i = 0; i < size; i++) {
        std::swap(bytes[first * size + i], bytes[second * size + i]);
    }
}

bool find_unit_element_and_swap(utils::seq_bytes &bytes, size_t size, int row, MatrixElimination matrix_elimination) {
    if (matrix_elimination == MatrixElimination::FORWARD) {
        int index = row + 1;
        while (index < size && bytes[index * size + row] == 0) {
            index++;
        }
        if (index < size) {
            swap_rows(bytes, size, row, index);
            return true;
        }
    } else {
        int index = row - 1;
        while (index >= 0 && bytes[index * size + row] == 0) {
            index--;
        }
        if (index >= 0) {
            swap_rows(bytes, size, row, index);
            return true;
        }
    }
    return false;
}

size_t BinaryMatrix::compute_rank() const {
    utils::seq_bytes copy_bytes = bytes;
    auto print_matrix = [&copy_bytes](size_t size) {
        for (size_t i = 0; i < size; i++) {
            for (size_t j = 0; j < size; j++) {
                std::cout << (int)copy_bytes[i * size + j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    };
    for (int i = 0; i < size - 1; i++) {
        if (copy_bytes[i * size + i] == 1) {
            elementary_row_operations(copy_bytes, size, i, MatrixElimination::FORWARD);
        } else {
            if (find_unit_element_and_swap(copy_bytes, size, i, MatrixElimination::FORWARD)) {
                elementary_row_operations(copy_bytes, size, i, MatrixElimination::FORWARD);
            }
        }
    }

    for (int i = size - 1; i > 0; i--) {
        if (copy_bytes[i * size + i] == 1) {
            elementary_row_operations(copy_bytes, size, i, MatrixElimination::BACKWARD);
        } else {
            if (find_unit_element_and_swap(copy_bytes, size, i, MatrixElimination::BACKWARD)) {
                elementary_row_operations(copy_bytes, size, i, MatrixElimination::BACKWARD);
            }
        }
    }

    size_t rank = size;
    for (size_t i = 0; i < size; i++) {
        bool all_zeros = true;
        for (size_t j = 0; j < size; j++) {
            if (copy_bytes[i * size + j] == 1) {
                all_zeros = false;
                break;
            }
        }
        if (all_zeros) {
            rank--;
        }
    }

    return rank;
}

void BinaryMatrix::print() const {
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            std::cout << (int)bytes[i * size + j] << " ";
        }
        std::cout << std::endl;
    }
}
