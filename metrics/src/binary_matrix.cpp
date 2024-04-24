#include "metrics/binary_matrix.hpp"

BinaryMatrix::BinaryMatrix() : size(0) {
}

BinaryMatrix::BinaryMatrix(const utils::seq_bytes bytes, size_t size) : bytes(bytes), size(size) {
}

size_t BinaryMatrix::rank_computation() const {
    utils::seq_bytes copy_bytes = bytes;
    for (size_t i = 0; i < size; i++) {
        if (copy_bytes[i * size + i] == 1) {
            for (size_t row = i + 1; row < size; row++) {
                if (copy_bytes[row * size + i] == 1) {
                    for (size_t col = i; col < size; col++) {
                        copy_bytes[row * size + col] = copy_bytes[row * size + col] ^ copy_bytes[i * size + col];
                    }
                }
            }
        } else {
            size_t first_one_index = i + 1;
            while (first_one_index < size && copy_bytes[first_one_index * size + i] != 1) {
                first_one_index++;
            }
            if (first_one_index < size) {
                for (size_t j = 0; j < size; j++) {
                    std::swap(copy_bytes[i * size + j], copy_bytes[first_one_index * size + j]);
                }
            } else {
                first_one_index = i + 1;
                while (first_one_index < size && copy_bytes[i * size + first_one_index] != 1) {
                    first_one_index++;
                }
                if (first_one_index < size) {
                    for (size_t j = 0; j < size; j++) {
                        std::swap(copy_bytes[j * size + i], copy_bytes[j * size + first_one_index]);
                    }
                    i--;
                }
            }
        }
    }

    size_t result = 0;
    for (size_t i = 0; i < size; i++) {
        bool have_one = false;
        for (size_t j = 0; j < size; j++) {
            if (copy_bytes[i * size + j] == 1) {
                have_one = true;
                break;
            }
        }
        if (have_one) {
            result++;
        }
    }
    return result;
}

void BinaryMatrix::print() const {
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            std::cout << bytes[i * size + j] << " ";
        }
        std::cout << std::endl;
    }
}