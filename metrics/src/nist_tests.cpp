#include <cmath>
#include <complex>
#include <iostream>
#include <map>
#include <unordered_map>
#include <utility>

#include <boost/math/special_functions/gamma.hpp>
#include <boost/math/special_functions/hypergeometric_1F1.hpp>

#include "metrics/binary_matrix.hpp"
#include "metrics/nist_tests.hpp"

constexpr double alpha = 0.01;

std::map<std::pair<size_t, size_t>, std::vector<double>> PI = {
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
            sum++;
        } else {
            sum--;
        }
    }
    double s_obs = std::abs(sum) / std::sqrt(length_bytes);
    return std::erfcl(s_obs / std::sqrt(2));
}

bool nist::check_frequency_test(const utils::seq_bytes &bytes) {
    return nist::frequency_test(bytes) >= alpha;
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
    return nist::frequency_block_test(bytes, count_part) >= alpha;
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
    return nist::runs_test(bytes) >= alpha;
}

double nist::longest_run_of_ones(const utils::seq_bytes &bytes) {
    std::map<size_t, size_t> M = {{128, 8}, {6572, 128}, {750000, 10000}};
    std::unordered_map<size_t, size_t> K = {{8, 3}, {128, 5}, {10000, 6}};
    std::unordered_map<size_t, size_t> N = {{8, 16}, {128, 49}, {10000, 75}};
    std::unordered_map<size_t, std::vector<std::uint16_t>> V = {{8, {1, 4}}, {128, {4, 9}}, {10000, {10, 16}}};
    size_t size_block = 0U;
    size_t length_bytes = bytes.size();
    for (const auto &[key, value] : M) {
        if (key >= length_bytes) {
            size_block = value;
            break;
        }
    }
    std::vector<size_t> v;
    std::vector<std::uint16_t> &bounds = V[size_block];
    v.resize(bounds[1] - bounds[0] + 1);
    size_t count_block = length_bytes / size_block;
    for (size_t i = 0; i < count_block; ++i) {
        size_t left_border = i * size_block;
        size_t right_border = left_border + size_block;
        size_t max_run = utils::get_max_run(bytes, left_border, right_border);
        if (max_run <= bounds[0]) {
            v.front()++;
        } else if (max_run >= bounds[1]) {
            v.back()++;
        } else {
            v[max_run - bounds[0]]++;
        }
    }
    double kappa = 0;
    size_t n = N[size_block];
    size_t k = K[size_block];
    std::vector<double> pi = PI[{k, size_block}];
    for (size_t i = 0; i <= k; ++i) {
        double temp = (v[i] - n * pi[i]);
        kappa += temp * temp / (n * pi[i]);
    }
    return boost::math::gamma_q(static_cast<double>(k) / 2.0, kappa / 2.0);
}

bool nist::check_longest_run_of_ones(const utils::seq_bytes &bytes) {
    return nist::longest_run_of_ones(bytes) >= alpha;
}

double nist::binary_matrix_rank(const utils::seq_bytes &bytes, size_t M, size_t Q) {
    if (M != Q) {
        throw std::runtime_error("Binary matrix rank test: M != Q\n");
    }
    size_t N = bytes.size() / (M * Q);
    std::vector<BinaryMatrix> binary_matrixes(N);
    size_t index = 0;
    for (size_t i = 0; i < N; i++) {
        utils::seq_bytes temp(M * Q);
        for (size_t j = 0; j < M * Q; j++, index++) {
            temp[j] = bytes[index];
        }
        binary_matrixes[i] = BinaryMatrix(temp, M);
    }
    std::vector<size_t> F(3, 0);
    for (size_t i = 0; i < N; i++) {
        // binary_matrixes[i].print();
        size_t rank_matrix = binary_matrixes[i].rank_computation();
        // std::cout << rank_matrix << std::endl << std::endl;
        if (rank_matrix == M) {
            F[0]++;
        } else if (rank_matrix == (M - 1)) {
            F[1]++;
        } else {
            F[2]++;
        }
    }
    double a = 0.2888 * N;
    double b = 0.5776 * N;
    double c = 0.1336 * N;
    double kappa = ((F[0] - a) * (F[0] - a)) / a + ((F[1] - b) * (F[1] - b)) / b + ((F[2] - c) * (F[2] - c)) / c;
    double result = std::exp(-kappa / 2);
    return result;
}

bool nist::check_binary_matrix_rank(const utils::seq_bytes &bytes, size_t M, size_t Q) {
    return nist::binary_matrix_rank(bytes, M, Q) >= alpha;
}

double nist::discrete_fourier_transform(const utils::seq_bytes &bytes) {
    size_t size = bytes.size();
    std::vector<short> x(size);
    for (size_t i = 0; i < size; i++) {
        x[i] = 2 * bytes[i] - 1;
    }
    std::vector<std::complex<double>> X = utils::DFT(x);
    std::vector<double> M(size);
    for (size_t i = 0; i < size; i++) {
        M[i] = std::abs(X[i]);
    }
    double T = std::sqrt(std::log(1 / 0.05) * size);
    std::cout << T << std::endl;
    double N_0 = 0.95 * size / 2;
    std::cout << N_0 << std::endl;
    double N_1 = 0;
    for (size_t i = 0; i < size / 2; i++) {
        if (M[i] < T) {
            N_1++;
        }
    }
    std::cout << N_1 << std::endl;
    double d = std::abs((N_1 - N_0) / std::sqrt(size * 0.95 * 0.05 / 4));
    std::cout << d << std::endl;
    return boost::math::erfc(d / std::sqrt(2));
}

bool nist::check_discrete_fourier_transform(const utils::seq_bytes &bytes) {
    return nist::discrete_fourier_transform(bytes) >= alpha;
}

double nist::non_overlapping_template_matching(const utils::seq_bytes &bytes, const utils::seq_bytes &template_,
                                               size_t N) {
    size_t n = bytes.size();
    size_t m = template_.size();
    size_t M = n / N;
    std::vector<utils::seq_bytes> blocks(N);
    for (size_t i = 0; i < N; i++) {
        for (size_t j = i * M; j < (i + 1) * M; j++) {
            blocks[i].push_back(bytes[j]);
        }
    }
    std::vector<size_t> W(N, 0);
    for (size_t i = 0; i < N; i++) {
        size_t index = 0;
        auto &current_block = blocks[i];
        while (index <= M - m) {
            bool match = true;
            size_t temp = index;
            for (size_t j = 0; j < m; j++) {
                if (current_block[temp] != template_[j]) {
                    match = false;
                    index++;
                    break;
                }
                temp++;
            }
            if (match) {
                W[i]++;
                index += m;
            }
        }
    }
    double pow2 = std::pow(2, m);
    double a = (M - m + 1) / pow2;
    double b = M * (1 / pow2 - (2 * m - 1) / (pow2 * pow2));
    double kappa = 0;
    for (size_t i = 0; i < N; i++) {
        kappa += (W[i] - a) * (W[i] - a);
    }
    kappa /= b;
    return boost::math::gamma_q(static_cast<double>(N) / 2.0, kappa / 2.0);
}

bool nist::check_non_overlapping_template_matching(const utils::seq_bytes &bytes, const utils::seq_bytes &template_,
                                                   size_t N) {
    return nist::non_overlapping_template_matching(bytes, template_, N) >= alpha;
}

double nist::overlapping_template_matching(const utils::seq_bytes &bytes, const utils::seq_bytes &template_, size_t N,
                                           size_t K) {
    size_t n = bytes.size();
    size_t m = template_.size();
    size_t M = n / N;
    std::vector<utils::seq_bytes> blocks(N);
    for (size_t i = 0; i < N; i++) {
        for (size_t j = i * M; j < (i + 1) * M; j++) {
            blocks[i].push_back(bytes[j]);
        }
    }
    std::vector<size_t> W(N, 0);
    for (size_t i = 0; i < N; i++) {
        size_t index = 0;
        auto &current_block = blocks[i];
        while (index <= M - m) {
            bool match = true;
            size_t temp = index;
            for (size_t j = 0; j < m; j++) {
                if (current_block[temp] != template_[j]) {
                    match = false;
                    break;
                }
                temp++;
            }
            if (match) {
                W[i]++;
            }
            index++;
        }
    }
    std::vector<size_t> v(K + 1, 0);
    for (size_t i = 0; i < N; i++) {
        if (W[i] > K + 1) {
            continue;
        }
        v[W[i]]++;
    }
    double lambda = (M - m + 1) / std::pow(2, m);
    double theta = lambda / 2;
    std::vector<double> pi(K + 1);
    if (K == 5) {
        pi[0] = 0.324652;
        pi[1] = 0.182617;
        pi[2] = 0.142670;
        pi[3] = 0.106645;
        pi[4] = 0.077147;
        pi[5] = 0.166269;
    } else {
        double a = std::exp(-2 * theta) * theta;
        for (size_t i = 0; i < K + 1; i++) {
            double b = std::pow(2, i);
            pi[i] = a / b * boost::math::hypergeometric_1F1(double(i + 1), double(2), theta);
        }
    }
    double kappa = 0;
    for (size_t i = 0; i < K + 1; i++) {
        kappa += (v[i] - N * pi[i]) * (v[i] - N * pi[i]) / (N * pi[i]);
    }
    return boost::math::gamma_q(K / 2.0, kappa / 2.0);
}

bool nist::check_overlapping_template_matching(const utils::seq_bytes &bytes, const utils::seq_bytes &template_,
                                               size_t N, size_t K) {
    return nist::overlapping_template_matching(bytes, template_, N, K) >= alpha;
}

double nist::universal(const utils::seq_bytes &bytes, size_t L, size_t Q) {
}

bool nist::check_universal(const utils::seq_bytes &bytes, size_t L, size_t Q) {
    return nist::universal(bytes, L, Q) >= alpha;
}
