#include <cmath>
#include <iostream>
#include <map>
#include <unordered_map>
#include <utility>

#include "metrics/diehard_tests.hpp"
#include "diehard_tests.hpp"

double diehard::runs_test(const utils::seq_bytes &bytes) {
    // Runs test
    size_t sample_size = bytes.size();
    size_t n1 = 0;
    for (const auto &byte : bytes) {
        if (byte) {
            ++n1;
        }
    }
    size_t n2 = sample_size - n1;
    double expected_runs = 1 + (2 * n1 * n2) / (n1 + n2);
    double standard_deviation = std::abs(((2* n1 * n2) * (2 * n1 * n2 - n1 - n2)) /
                                        double(std::pow(n1 + n2, 2) * (n1 + n2 - 1)));
    size_t runs = 1;
    for (size_t i = 0; i < sample_size; i++) {
        if (bytes[i] != bytes[i + 1]) {
            ++runs;
        }
    }
    // If abs of return value > 1.96 -> Not random (5% significance)
    return std::erfcl((runs - expected_runs) / standard_deviation);
}


double matrix_test(const utils::seq_bytes &bytes, int rows, int cols, int iterations) {    
    // This test calculates ranks of matrices and performs chi-square test on them
    int offset = 0;
    std::vector<int> ranks;
    std::map<int, int> map_ranks;
    for (size_t i = 0; i < iterations; i++){
        std::vector<std::vector<int>> matrix = utils::matrix_from_bytes(bytes, rows, cols, offset);
        offset += rows * cols;
        int rank = utils::binary_matrix_rank(matrix, cols, rows);
        ranks.push_back(rank);
        if (map_ranks.find(rank) == map_ranks.end()) {
            map_ranks[rank] = 1;
        } else {
            map_ranks[rank]++;
        }
    }
    // PROBABILITIES FOR RANKS CAN BE CALCULATED. See https://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-22r1a.pdf  page 67-68
    
}
