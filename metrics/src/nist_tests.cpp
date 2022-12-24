#pragma once

#include <boost/math/special_functions/gamma.hpp>
#include <cmath>
#include <iostream>
#include <map>
#include <unordered_map>
#include <utility>


#include "metrics/nist_tests.hpp"

std::map<std::pair<std::int64_t, std::int64_t>, std::vector<double>> PI = {
    {{3, 8}, {0.2148, 0.3672, 0.2305, 0.1875}},
    {{5, 128}, {0.1174, 0.2430, 0.2493, 0.1752, 0.1027, 0.1124}},
    {{5, 512}, {0.1170, 0.2460, 0.2523, 0.1755, 0.1027, 0.1124}},
    {{5, 10000}, {0.1307, 0.2437, 0.2452, 0.1714, 0.1002, 0.1088}},
    {{6, 10000}, {0.0882, 0.2092, 0.2483, 0.1933, 0.1208, 0.0675, 0.0727}}};

double nist::frequency_test(const utils::seq_bytes &bytes) {
    size_t length_bytes = bytes.size();
    std::int64_t sum = 0;
    for (const auto &byte : bytes) {
        if (byte) {
            sum += 1;
        } else {
            sum -= 1;
        }
    }
    double s_obs = std::abs(sum) / std::sqrt(length_bytes);
    return std::erfcl(s_obs / std::sqrt(2));
}

bool nist::check_frequency_test(const utils::seq_bytes &bytes) {
    return nist::frequency_test(bytes) >= 0.01;
}

double nist::frequency_block_test(const utils::seq_bytes &bytes, size_t count_part) {
    size_t length_bytes = bytes.size();
    size_t size_block = length_bytes / count_part;
    std::vector<double> pi;
    pi.resize(count_part);
    for (size_t i = 0; i < count_part; ++i) {
        size_t sum = 0;
        size_t left_border = i * size_block;
        size_t right_border = left_border + size_block;
        for (size_t j = left_border; j < right_border; ++j) {
            if (bytes[j]) {
                ++sum;
            }
        }
        pi[i] = static_cast<double>(sum) / count_part;
    }
    double kappa = 4 * count_part;
    double sum = 0;
    for (const auto &p : pi) {
        sum += (p - 0.5) * (p - 0.5);
    }
    kappa *= sum;
    return boost::math::gamma_q(static_cast<double>(size_block) / 2.0, kappa / 2.0);
}

bool nist::check_frequency_block_test(const utils::seq_bytes &bytes, size_t count_part) {
    return nist::frequency_block_test(bytes, count_part) >= 0.01;
}

double nist::runs_test(const utils::seq_bytes &bytes) {
    if (!nist::check_frequency_test(bytes)) {
        return 0.0;
    }
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

bool nist::check_runs_test(const utils::seq_bytes &bytes) {
    return nist::runs_test(bytes) >= 0.01;
}

double nist::longest_run_of_ones(const utils::seq_bytes &bytes) {
    std::unordered_map<std::uint64_t, std::uint64_t> M = {{128, 8}, {6572, 128}, {750000, 10000}};
    std::unordered_map<std::uint64_t, std::uint64_t> K = {{8, 3}, {128, 5}, {10000, 6}};
    std::unordered_map<std::uint64_t, std::uint64_t> N = {{8, 16}, {128, 49}, {10000, 75}};
    std::unordered_map<std::uint64_t, std::vector<std::uint16_t>> V = {{8, {1, 4}}, {128, {4, 9}}, {10000, {10, 16}}};
    std::uint64_t size_block = 0U;
    size_t length_bytes = bytes.size();
    for (const auto &x : M) {
        if (x.first >= length_bytes) {
            size_block = x.second;
            break;
        }
    }
    std::vector<std::uint64_t> v;
    std::vector<std::uint16_t> &bounds = V[size_block];
    v.resize(bounds[1] - bounds[0] + 1);
    std::size_t count_block = length_bytes / size_block;
    for (size_t i = 0; i < count_block; ++i) {
        size_t left_border = i * size_block;
        size_t right_border = left_border + size_block;
        size_t max_run = utils::get_max_run(bytes, left_border, right_border);
        if (max_run <= bounds[0]) {
            v.front() += 1U;
        } else if (max_run >= bounds[1]) {
            v.back() += 1U;
        } else {
            v[max_run - bounds[0]] += 1U;
        }
    }
    double kappa = 0;
    std::uint64_t n = N[size_block];
    std::uint64_t k = K[size_block];
    std::vector<double> pi = PI[{k, size_block}];
    for (size_t i = 0; i <= k; ++i) {
        double temp = (v[i] - n * pi[i]);
        kappa += temp * temp / (n * pi[i]);
    }
    return boost::math::gamma_q(static_cast<double>(k) / 2.0, kappa / 2.0);
}

bool nist::check_longest_run_of_ones(const utils::seq_bytes &bytes) {
    return nist::longest_run_of_ones(bytes) >= 0.01;
}
