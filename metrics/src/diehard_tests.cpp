#include <algorithm>
#include <bitset>
#include <boost/math/distributions/chi_squared.hpp>
#include <boost/math/distributions/normal.hpp>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
#include <unordered_map>
#include <utility>

#include "diehard_const.hpp"
#include "diehard_tests.hpp"
#include "metrics/binary_matrix.hpp"
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
    std::vector<size_t> ranks;
    std::map<int, int> map_ranks;

    for (size_t i = 0; i < iterations; i++) {
        utils::seq_bytes slice(bytes.begin() + offset, bytes.begin() + offset + rows * cols);

        BinaryMatrix matrix(slice, rows);
        offset += rows * cols;
        size_t rank = matrix.compute_rank();
        ranks.push_back(rank);
        if (map_ranks.find(rank) == map_ranks.end()) {
            map_ranks[rank] = 1;
        } else {
            map_ranks[rank]++;
        }
    }
    std::map<int, double> map_probs;
    for (const auto &p : map_ranks) {
        double probability = matrix_rank_prob(rows, cols, p.first);
        map_probs[p.first] = probability;
    }
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
    // double p_value = utils::p_value(degrees_of_freedom, chi_square);
    double p_value = boost::math::gamma_q(((double)(degrees_of_freedom) / 2.0), chi_square / 2.0);

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

    std::vector<double> expected_histogram(kmax, 0);
    for (size_t k = 0; k < kmax; k++) {
        expected_histogram[k] = tsamples * utils::poissonian(k, lambda);
    }

    // Calculate chi_square and p-value
    double chi_square = utils::chi_square(histogram, expected_histogram, kmax);
    // double p_value = utils::p_value((double)(kmax - 1) / 2.0, chi_square / 2.0);
    double p_value = boost::math::gamma_q(((double)(kmax - 1) / 2.0), chi_square / 2.0);

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
        std::vector<double> doubles = utils::bits_to_doubles(bytes, num_coordinates * n_dims);
        for (size_t dot_num = 0; dot_num < num_coordinates; dot_num++) {
            std::vector<double> dot;
            for (size_t dim = 0; dim < n_dims; dim++) {
                dot.push_back(doubles[dot_num * n_dims + dim]);
            }
            coordinates.push_back(dot);
        }
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
    std::vector<uint> data = utils::bits_to_vector_uint<uint>(bytes, num_samples + 4);
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
    // double p_value = utils::p_value(degrees_of_freedom / 2.0, chi_square / 2.0);
    double p_value = boost::math::gamma_q(((double)(degrees_of_freedom) / 2.0), chi_square / 2.0);

    return p_value;
}

double diehard::base_5_word_chi_sq(const utils::seq_bytes &bytes, int num_samples, int word_length) {
    const double probabilities[5] = {37.0 / 256, 56.0 / 256, 70.0 / 256, 56.0 / 256, 37.0 / 256};
    // const double mu = 2500, std = 70.7106781;
    auto count_ones = [](uint8_t byte) {
        int count = 0;
        while (byte) {
            count += byte & 1;
            byte >>= 1;
        }
        return count;
    };

    auto map_to_letter = [](int ones) {
        if (ones <= 2)
            return 'A';
        if (ones == 3)
            return 'B';
        if (ones == 4)
            return 'C';
        if (ones == 5)
            return 'D';
        return 'E';
    };

    std::unordered_map<std::string, int> word_count;
    for (size_t i = 0; i <= num_samples; ++i) {
        std::string word;
        for (int j = 0; j < word_length; ++j) {
            int ones = count_ones(utils::bits_to_uint<uint8_t>(bytes, i + j));
            word += map_to_letter(ones);
        }
        word_count[word]++;
    }
    std::vector<double> observed;
    std::vector<double> expected;

    for (const auto &entry : word_count) {
        const std::string &word = entry.first;
        int observed_count = entry.second;
        observed.push_back(observed_count);

        double expected_count = 1.0;
        for (char c : word) {
            if (c == 'A')
                expected_count *= probabilities[0];
            else if (c == 'B')
                expected_count *= probabilities[1];
            else if (c == 'C')
                expected_count *= probabilities[2];
            else if (c == 'D')
                expected_count *= probabilities[3];
            else if (c == 'E')
                expected_count *= probabilities[4];
        }
        expected_count *= num_samples;
        expected.push_back(expected_count);
    }
    int degrees_of_freedom = observed.size() - 1;
    double chi_sq = utils::chi_square(observed, expected, degrees_of_freedom);
    return chi_sq;
}

double diehard::monkey_test(const utils::seq_bytes &bytes, int num_samples) {
    const double mu = 2500, std = 70.7106781;

    double chi_sq_len_5 = base_5_word_chi_sq(bytes, num_samples, 5);
    double chi_sq_len_4 = base_5_word_chi_sq(bytes, num_samples, 4);
    boost::math::normal_distribution<double> normal_dist(mu, std);
    double p_value = boost::math::cdf(normal_dist, chi_sq_len_5 - chi_sq_len_4);
    // std::cout << "P-Value: " << p_value << std::endl;
    return p_value;
}

double diehard::squeeze_test(const utils::seq_bytes &bytes, int num_samples) {
    std::vector<double> doubles = utils::bits_to_doubles(bytes, num_samples * 50);
    const size_t initial_k = (1u << 31) - 1;
    const size_t max_bins = 48;
    const size_t min_bins = 6;
    static double probabilities[] = {
        0.00002103, 0.00005779, 0.00017554, 0.00046732, 0.00110783, 0.00236784, 0.00460944, 0.00824116, 0.01362781,
        0.02096849, 0.03017612, 0.04080197, 0.05204203, 0.06283828, 0.07205637, 0.07869451, 0.08206755, 0.08191935,
        0.07844008, 0.07219412, 0.06398679, 0.05470931, 0.04519852, 0.03613661, 0.02800028, 0.02105567, 0.01538652,
        0.01094020, 0.00757796, 0.00511956, 0.00337726, 0.00217787, 0.00137439, 0.00084970, 0.00051518, 0.00030666,
        0.00017939, 0.00010324, 0.00005851, 0.00003269, 0.00001803, 0.00000982, 0.00001121};
    std::vector<double> expected(43);
    for (size_t i = 0; i < 43; ++i) {
        expected[i] = probabilities[i] * num_samples;
    }

    std::vector<double> bins(43, 0.0);
    int global_iterations = 0;
    for (int sample = 0; sample < num_samples; ++sample) {
        int k = initial_k;
        int iterations = 0;
        // while (k > 1.0) {
        while ((k > 1.0) && (iterations < 49)) {
            double U = doubles[global_iterations + iterations];
            // std::cout << k << " * " << U << std::endl;
            k = static_cast<int>(std::ceil(k * U));
            iterations++;
        }

        if (iterations <= min_bins) {
            bins[0]++;
        } else if (iterations >= max_bins) {
            bins[max_bins - 6]++;
        } else {
            bins[iterations - 6]++;
        }
        global_iterations += iterations;
    }
    double chi_square = utils::chi_square(bins, expected, 42);
    // double p_value = utils::p_value(max_bins - min_bins - 1, chi_square);
    double p_value = boost::math::gamma_q(((double)(max_bins - min_bins - 1) / 2.0), chi_square / 2.0);

    // for (size_t i = 0; i < 43; ++i) {
    //     std::cout << "[" << i << "] "
    //               << "Expected: " << expected[i] << " -- Trial: " << bins[i] << std::endl;
    // }

    return p_value;
}

double diehard::sums_test(const utils::seq_bytes &bytes, int num_samples) {
    std::vector<double> doubles = utils::bits_to_doubles(bytes, num_samples);
    double sum = 0.0;
    for (size_t i = 0; i < num_samples; i++) {
        sum += doubles[i];
    }
    double expected = num_samples * 0.5;
    double std = sqrt(num_samples / 12.0);
    // std::cout << "Expected: " << expected << " | Trial: " << sum << std::endl;
    boost::math::normal_distribution<double> normal_dist(0, std);
    double p_value = boost::math::cdf(normal_dist, expected - sum);
    // std::cout << "P-Value: " << p_value << std::endl;
    return p_value;
}

double diehard::craps_test(const utils::seq_bytes &bytes, int num_games) {
    std::vector<int> wins(0);
    std::vector<int> throws(0);

    // Convert bytes to numbers we can use for dice rolls
    std::vector<uint> rolls = utils::bits_to_vector_uint<uint>(bytes, num_games * 2);

    for (int game = 0; game < num_games; game++) {

        int throws_this_game = 1;

        // First throw - sum of two dice
        int die1 = (rolls[game * 2] % 6) + 1;
        int die2 = (rolls[game * 2 + 1] % 6) + 1;
        int sum = die1 + die2;

        bool game_won = false;
        bool game_over = false;

        // Check first throw results
        if (sum == 7 || sum == 11) {
            game_won = true;
            game_over = true;
        } else if (sum == 2 || sum == 3 || sum == 12) {
            game_won = false;
            game_over = true;
        }

        // If game isn't over, we have a point
        int point = sum;
        while (!game_over) {
            throws_this_game++;

            // Get next roll
            die1 = (rolls[game * 2 + throws_this_game * 2] % 6) + 1;
            die2 = (rolls[game * 2 + throws_this_game * 2 + 1] % 6) + 1;
            sum = die1 + die2;

            if (sum == point) {
                game_won = true;
                game_over = true;
            } else if (sum == 7) {
                game_won = false;
                game_over = true;
            }
        }

        wins.push_back(game_won ? 1 : 0);
        throws.push_back(throws_this_game);
    }

    // Count total wins
    int total_wins = std::accumulate(wins.begin(), wins.end(), 0);

    // Expected values
    double p = 244.0 / 495.0;
    double expected_wins = num_games * p;
    double variance = num_games * p * (1.0 - p);
    double std_dev = std::sqrt(variance);

    // Calculate z-score
    double z_score = std::abs(total_wins - expected_wins) / std_dev;

    // Convert to p-value using erfc
    double p_value = std::erfc(z_score / std::sqrt(2.0));
    return p_value;
}
