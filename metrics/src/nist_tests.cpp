#pragma once

#include <boost/math/special_functions/gamma.hpp>
#include <cmath>
#include <iostream>
#include <unordered_map>


#include "metrics/nist_tests.hpp"

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
