#include <boost/math/distributions/chi_squared.hpp>
#include <cmath>
#include <iostream>
#include <map>
#include <unordered_map>
#include <utility>

#include "diehard_tests.hpp"
#include "metrics/diehard_tests.hpp"
#include "metrics/utils.hpp"

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
    double standard_deviation =
        std::abs(((2 * n1 * n2) * (2 * n1 * n2 - n1 - n2)) / double(std::pow(double(n1 + n2), 2) * (n1 + n2 - 1)));
    size_t runs = 1;
    for (size_t i = 0; i < sample_size; i++) {
        if (bytes[i] != bytes[i + 1]) {
            ++runs;
        }
    }
    // If abs of return value > 1.96 -> Not random (5% significance)
    return std::erfcl((runs - expected_runs) / standard_deviation);
}

double diehard::matrix_rank_prob(int M, int Q, int rank) {
    /*
    Calculates the probability that matrix rank is 'rank'
    M = rows
    Q = cols
    */
    int m = std::min(M, Q);
    double probability;
    double first_part = 2 << (rank * (Q + M - rank) - M * Q);
    double second_part = 1.0;
    for (size_t i = 0; i < rank; ++i) {
        double numerator = (1 - (2 << (i - Q))) * (1 - (2 << (i - M)));
        double denominator = 1 - (2 << (i - rank));
        second_part *= numerator / denominator;
    }
    probability = first_part * second_part;
    return probability;
}

double diehard::matrix_test(const utils::seq_bytes &bytes, int rows, int cols, int iterations, int &degrees_of_freedom) {
    /*
    Calculates ranks of matrices and performs chi-square test on them
    degrees_of_freedom - number of degrees of freedom. Equal number of different ranks - 1
    */
    int offset = 0;
    std::vector<int> ranks;
    std::map<int, int> map_ranks;
    for (size_t i = 0; i < iterations; i++) {
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
    for (const auto &p : map_ranks) {
        std::cout << p.first << ": " << p.second << std::endl;
    }
    std::map<int, double> map_probs;
    for (const auto &p : map_ranks) {
        double probability = matrix_rank_prob(rows, cols, p.first);
        map_probs[p.first] = probability;
        degrees_of_freedom++;
    }
    for (const auto &p : map_probs) {
        std::cout << "Probs: " << p.first << ": " << p.second << std::endl;
    }
    boost::math::chi_squared_distribution<> chi_squared(degrees_of_freedom);
    double chi_squared_score = 0;
    for (const auto &p : map_probs) {
        chi_squared_score +=
            std::pow(map_ranks[p.first] - map_probs[p.first] * iterations, 2) / (map_probs[p.first] * iterations);
    }
    // double cdf = boost::math::cdf(chi_squared, chi_squared_score);
    // std::cout << "degrees_of_freedom: " << degrees_of_freedom << std::endl;
    std::cout << "chi_squared_score" << chi_squared_score << std::endl;
    // std::cout << "cdf: " << cdf << std::endl;
    return std::exp(-chi_squared_score / 2);
}
