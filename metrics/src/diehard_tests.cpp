#include <algorithm>
#include <bitset>
#include <boost/math/distributions/chi_squared.hpp>
#include <cmath>
#include <iostream>
#include <map>
#include <unordered_map>
#include <utility>

#include "diehard_const.hpp"
#include "diehard_tests.hpp"
#include "metrics/diehard_tests.hpp"
#include "metrics/utils.hpp"

double diehard::runs_test(const utils::seq_bytes &bytes) {
    size_t length_bytes = bytes.size();
    double pi = 0;
    for (const auto &byte : bytes) {
        if (byte) {
            ++pi;
        }
    }
    pi /= length_bytes;
    size_t v = 1;
    for (size_t i = 0; i < length_bytes - 1; ++i) {
        if (bytes[i] != bytes[i + 1]) {
            ++v;
        }
    }
    return std::erfcl(std::abs(v - 2 * length_bytes * pi * (1 - pi)) /
                      (2.0 * std::sqrt(2 * length_bytes) * pi * (1 - pi)));
}

double diehard::matrix_rank_prob(int M, int Q, int rank) {
    /*
    Calculates the probability that matrix rank is 'rank'
    M = rows
    Q = cols
    */
    int m = std::min(M, Q);
    double probability;
    double first_part = std::pow(2, (rank * (Q + M - rank) - M * Q));
    double second_part = 1.0;
    for (int i = 0; i < rank; ++i) {
        double numerator = (1 - std::pow(2, (i - Q))) * (1 - std::pow(2, (i - M)));
        double denominator = 1 - std::pow(2, (i - rank));
        // std::cout << "numerator: " << numerator << " = " << (1 - std::pow(2, (i - Q))) << " * " << (1 - std::pow(2,
        // (i - M))) << std::endl; std::cout << "denominator: " << denominator << " = " << 1 - std::pow(2, (i - rank))
        // << std::endl;
        second_part *= numerator / denominator;
    }
    probability = first_part * second_part;
    return probability;
}

double diehard::matrix_test(const utils::seq_bytes &bytes, int rows, int cols, int iterations) {
    /*
    Calculates ranks of matrices and performs chi-square test on them
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
    // for (const auto &p : map_ranks) {
    //     std::cout << p.first << ": " << p.second << std::endl;
    // }
    std::map<int, double> map_probs;
    for (const auto &p : map_ranks) {
        double probability = matrix_rank_prob(rows, cols, p.first);
        map_probs[p.first] = probability;
    }
    // for (const auto &p : map_probs) {
    //     std::cout << "Probs: " << p.first << ": " << p.second << std::endl;
    // }
    // Discard ranks with less than cutoff matrices
    int cutoff = std::min(iterations / 100, 5);
    int degrees_of_freedom = -1;
    std::vector<double> significant_ranks_trial;
    std::vector<double> significant_ranks_expected;
    for (const auto &p : map_ranks) {
        if (p.second > cutoff) {
            significant_ranks_trial.push_back(p.second);
            significant_ranks_expected.push_back(map_probs[p.first] * iterations);
            degrees_of_freedom++;
        }
    }
    double chi_square = utils::chi_square(significant_ranks_trial, significant_ranks_expected, degrees_of_freedom);
    // std::cout << "Chi squared value: " << chi_square << std::endl;
    double p_value = utils::p_value(degrees_of_freedom, chi_square);
    // std::cout << "p-value: " << p_value << std::endl;

    return p_value;
}

double diehard::birthdays_test(const utils::seq_bytes &bytes, int days_bits, int num_bdays, int tsamples) {
    // days_bits should be <= 32
    // bytes length should be >= days_bits * num_bdays + tsamples

    static double lambda = (double)num_bdays * num_bdays * num_bdays / std::pow(2.0, (double)days_bits + 2.0);
    int kmax = 1;
    while ((tsamples * utils::poissonian(kmax, lambda)) > 5) {
        kmax++;
    }
    kmax++;

    std::vector<double> histogram(kmax, 0.0);

    for (size_t sample = 0; sample < tsamples; sample++) {
        std::vector<unsigned int> uints;
        for (size_t i = 0; i < num_bdays; i++) {
            std::vector<bool> sub_vec(bytes.begin() + i * days_bits + sample,
                                      bytes.begin() + i * days_bits + sample + days_bits);
            std::bitset<32> bday_bitset(0);
            for (size_t j = 0; j < days_bits; j++) {
                bday_bitset[days_bits - 1 - j] = sub_vec[j];
            }
            uints.push_back(bday_bitset.to_ulong());
        }

        std::sort(uints.begin(), uints.end());

        // If num_bdays = 512 and days_bits = 24, lambda = 2
        // If num_bdays = 2048 and days_bits = 30, lambda = 2

        std::vector<unsigned int> intervals(num_bdays);
        intervals[0] = uints[0];
        for (size_t i = 1; i < num_bdays; i++) {
            intervals[i] = uints[i] - uints[i - 1];
        }
        std::sort(intervals.begin(), intervals.end());

        // Count how many intervals occur more than once
        int k = 0;
        for (int i_interval = 1; i_interval < num_bdays; ++i_interval) {
            if (intervals[i_interval] == intervals[i_interval - 1]) {
                ++k;
                while (i_interval < intervals.size() && intervals[i_interval] == intervals[i_interval - 1]) {
                    i_interval++;
                }
            }
        }

        if (k < kmax) {
            histogram[k] += 1.0;
        } else {
            histogram[k] += 1.0;
        }
    }
    // std::cout << "Histogram: " << std::endl;
    // for (size_t i = 0; i < tsamples; i++) {
    //     std::cout << i << ": " << histogram[i] << std::endl;
    // }

    std::vector<double> expected_histogram(kmax, 0);
    for (size_t k = 0; k < kmax; k++) {
        expected_histogram[k] = tsamples * utils::poissonian(k, lambda);
    }

    // std::cout << "Expected Histogram: " << std::endl;
    // for (size_t i = 0; i < tsamples; i++) {
    //     std::cout << i << ": " << expected_histogram[i] << std::endl;
    // }

    // Calculate chi_square and p-value
    double chi_square = utils::chi_square(histogram, expected_histogram, kmax);
    double p_value = utils::p_value((double)(kmax - 1) / 2.0, chi_square / 2.0);

    return p_value;
}

double diehard::minimum_distance_test(const utils::seq_bytes &bytes, int n_dims, int num_coordinates, int num_samples) {
    const double pi = boost::math::constants::pi<double>();
    static double rgb_md_Q[] = {0.0, 0.0, 0.4135, 0.5312, 0.6202, 1.3789};
    std::vector<double> p_values;
    for (size_t sample = 0; sample < num_samples; sample++) {
        // Create num_coordinates dots in n_dims dimentional space
        std::vector<std::vector<double>> coordinates;
        auto start = bytes.begin() + sample * num_coordinates;
        auto end = bytes.begin() + (sample + 1) * num_coordinates;
        utils::seq_bytes sub_seq_bytes(start, end);
        // std::vector<double> doubles = utils::doubles_from_bits(sub_seq_bytes, num_coordinates * n_dims);
        std::vector<double> doubles = utils::random_doubles(num_coordinates * n_dims);
        for (size_t dot_num = 0; dot_num < num_coordinates; dot_num++) {
            std::vector<double> dot;
            for (size_t dim = 0; dim < n_dims; dim++) {
                dot.push_back(doubles[dot_num * n_dims + dim]);
            }
            coordinates.push_back(dot);
        }
        // for (size_t i = 0; i < num_coordinates; i++) {
        //     std::cout << "(" << coordinates[i][0] << ": " << coordinates[i][1] << "); ";
        // }
        // std::cout << std::endl;
        // Sort the dots by their first coordinate
        std::sort(coordinates.begin(), coordinates.end(),
                  [](const std::vector<double> &a, const std::vector<double> &b) { return a[0] < b[0]; });
        // Go through the coordinates and find the minimum distance between dots
        double minimal_distance = std::numeric_limits<double>::max();
        for (size_t i = 1; i < coordinates.size(); i++) {
            double distance = 0;
            for (size_t dim = 0; dim < n_dims; dim++) {
                distance += std::pow(coordinates[i][dim] - coordinates[i - 1][dim], 2);
            }
            distance = std::sqrt(distance);
            if (distance < minimal_distance) {
                minimal_distance = distance;
            }
        }
        // std::cout << "minimal_distance = " << minimal_distance << std::endl;

        double dvolume;
        if ((n_dims % 2) == 0) {
            dvolume = std::pow(pi, n_dims / 2) * std::pow(minimal_distance, n_dims) /
                      boost::math::factorial<double>(n_dims / 2);
        } else {
            dvolume = 2.0 * std::pow(2.0 * pi, (n_dims - 1) / 2) * std::pow(minimal_distance, n_dims) /
                      boost::math::double_factorial<double>(n_dims);
        }
        double earg = -1.0 * num_coordinates * (num_coordinates - 1) * dvolume / 2.0;
        double qarg = 1.0 + ((2.0 + rgb_md_Q[n_dims]) / 6.0) * std::pow(1.0 * num_coordinates, 3) * dvolume * dvolume;
        double p_value = 1.0 - std::exp(earg) * qarg;

        p_values.push_back(p_value);
    }
    return utils::kstest(p_values);
}

double diehard::overlapping_permutations_test(const utils::seq_bytes &bytes, int num_samples) {
    std::vector<uint> data = utils::bits_to_vector_uint(bytes, num_samples + 4);
    double count[120];
    double x[120];
    std::fill(count, count + 120, 0.0);
    for (size_t sample = 0; sample < num_samples; sample++) {
        std::vector<int> sub_data(data.begin() + sample, data.begin() + sample + 5);
        int perm_ind = utils::kperm(sub_data);

        count[perm_ind]++;
    }
    double chi_square = 0.0;
    double av = (double)(num_samples) / 120.0;
    double norm = num_samples;
    for (size_t i = 0; i < 120; i++) {
        x[i] = count[i] - av;
    }
    for (size_t i = 0; i < 120; i++) {
        for (size_t j = 0; j < 120; j++) {
            chi_square += x[i] * pseudoInv[i][j] * x[j];
        }
    }
    chi_square = std::abs(chi_square / norm);
    int degrees_of_freedom = 96;
    double p_value = utils::p_value(degrees_of_freedom / 2.0, chi_square / 2.0);

    return p_value;
}