#include <algorithm>
#include <cmath>
#include <complex>
#include <iostream>
#include <map>
#include <unordered_map>
#include <utility>

#include <boost/math/distributions/normal.hpp>
#include <boost/math/special_functions/gamma.hpp>
#include <boost/math/special_functions/hypergeometric_1F1.hpp>

#include "metrics/binary_matrix.hpp"
#include "metrics/nist_tests.hpp"

constexpr std::double_t alpha = 0.01;

std::double_t nist::frequency_test(const utils::seq_bytes &bytes) {
    size_t length_bytes = bytes.size();
    std::int64_t sum = 0;
    for (const auto &byte : bytes) {
        if (byte) {
            sum++;
        } else {
            sum--;
        }
    }
    std::double_t s_obs = std::abs(sum) / std::sqrt(length_bytes);
    return boost::math::erfc(s_obs / std::sqrt(2));
}

bool nist::check_frequency_test(const utils::seq_bytes &bytes) {
    return nist::frequency_test(bytes) >= alpha;
}

std::double_t nist::frequency_block_test(const utils::seq_bytes &bytes, size_t m) {
    size_t length_bytes = bytes.size();
    size_t count_block = length_bytes / m;
    std::vector<std::double_t> pi;
    pi.resize(count_block);
    for (size_t i = 0; i < count_block; ++i) {
        size_t sum = 0;
        size_t left_border = i * m;
        size_t right_border = left_border + m;
        for (size_t j = left_border; j < right_border; ++j) {
            sum += bytes[j];
        }
        pi[i] = static_cast<std::double_t>(sum) / m;
    }
    std::double_t kappa = 4 * m;
    std::double_t sum = 0;
    for (const auto &p : pi) {
        sum += (p - 0.5) * (p - 0.5);
    }
    kappa *= sum;
    return boost::math::gamma_q(static_cast<std::double_t>(count_block) / 2.0, kappa / 2.0);
}

bool nist::check_frequency_block_test(const utils::seq_bytes &bytes, size_t m) {
    return nist::frequency_block_test(bytes, m) >= alpha;
}

std::double_t nist::runs_test(const utils::seq_bytes &bytes) {
    if (!nist::check_frequency_test(bytes)) {
        return 0.0;
    }
    size_t length_bytes = bytes.size();
    std::double_t pi = 0;
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
    return boost::math::erfc(std::abs(v - 2 * length_bytes * pi * (1 - pi)) /
                             (2.0 * std::sqrt(2 * length_bytes) * pi * (1 - pi)));
}

bool nist::check_runs_test(const utils::seq_bytes &bytes) {
    return nist::runs_test(bytes) >= alpha;
}

std::double_t nist::longest_run_of_ones(const utils::seq_bytes &bytes) {
    std::map<std::pair<size_t, size_t>, std::vector<std::double_t>> PI = {
        {{3, 8}, {0.2148, 0.3672, 0.2305, 0.1875}},
        {{5, 128}, {0.1174, 0.2430, 0.2493, 0.1752, 0.1027, 0.1124}},
        {{6, 10000}, {0.0882, 0.2092, 0.2483, 0.1933, 0.1208, 0.0675, 0.0727}}};
    std::map<size_t, size_t> M = {{128, 8}, {6572, 128}, {750000, 10000}};
    std::unordered_map<size_t, size_t> K = {{8, 3}, {128, 5}, {10000, 6}};
    std::unordered_map<size_t, std::vector<std::uint16_t>> V = {{8, {1, 4}}, {128, {4, 9}}, {10000, {10, 16}}};
    size_t size_block = 0U;
    size_t length_bytes = bytes.size();
    if (length_bytes < 6572) {
        size_block = 8;
    } else if (length_bytes < 750000) {
        size_block = 128;
    } else {
        size_block = 10000;
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
    std::double_t kappa = 0;
    size_t k = K[size_block];
    std::vector<std::double_t> pi = PI[{k, size_block}];
    for (size_t i = 0; i <= k; ++i) {
        std::double_t temp = (v[i] - count_block * pi[i]);
        kappa += temp * temp / (count_block * pi[i]);
    }
    return boost::math::gamma_q(static_cast<std::double_t>(k) / 2.0, kappa / 2.0);
}

bool nist::check_longest_run_of_ones(const utils::seq_bytes &bytes) {
    return nist::longest_run_of_ones(bytes) >= alpha;
}

std::double_t nist::binary_matrix_rank(const utils::seq_bytes &bytes, size_t M, size_t Q) {
    if (M != Q) {
        throw std::runtime_error("Binary matrix rank test: M != Q\n");
    }
    size_t N = bytes.size() / (M * Q);
    std::vector<BinaryMatrix> binary_matrixes(N);
    size_t index = 0;
    for (size_t i = 0; i < N; ++i) {
        utils::seq_bytes temp(M * Q);
        for (size_t j = 0; j < M * Q; ++j) {
            temp[j] = bytes[index];
            index++;
        }
        binary_matrixes[i] = BinaryMatrix(temp, M);
    }
    std::vector<size_t> F(3, 0);
    for (size_t i = 0; i < N; ++i) {
        size_t rank_matrix = binary_matrixes[i].compute_rank();
        if (rank_matrix == M) {
            F[0]++;
        } else if (rank_matrix == (M - 1)) {
            F[1]++;
        } else {
            F[2]++;
        }
    }
    std::double_t a = 0.288788 * N;
    std::double_t b = 0.577576 * N;
    std::double_t c = 0.133636 * N;
    std::double_t kappa = std::pow(F[0] - a, 2) / a + std::pow(F[1] - b, 2) / b + std::pow(F[2] - c, 2) / c;
    std::double_t result = std::exp(-kappa / 2);
    return result;
}

bool nist::check_binary_matrix_rank(const utils::seq_bytes &bytes, size_t M, size_t Q) {
    return nist::binary_matrix_rank(bytes, M, Q) >= alpha;
}

std::vector<short> normalized(const utils::seq_bytes &bytes) {
    size_t size = bytes.size();
    std::vector<short> x(size);
#pragma omp parallel for
    for (size_t i = 0; i < size; ++i) {
        x[i] = 2 * bytes[i] - 1;
    }
    return x;
}

std::double_t nist::discrete_fourier_transform(const utils::seq_bytes &bytes) {
    size_t size = bytes.size();
    std::vector<short> x = normalized(bytes);
    std::vector<std::complex<std::double_t>> X;
    if ((size & (size - 1)) == 0) {
        X = utils::FFT(x); // more fast for power of two
    } else {
        X = utils::DFT(x);
    }
    std::vector<std::double_t> M(size);
    for (size_t i = 0; i < size; ++i) {
        M[i] = std::abs(X[i]);
    }
    std::double_t T = std::sqrt(std::log(1 / 0.05) * size);
    std::double_t N_0 = 0.95 * size / 2;
    std::double_t N_1 = 0;
    for (size_t i = 0; i < size / 2; ++i) {
        if (M[i] < T) {
            N_1++;
        }
    }
    std::double_t d = std::abs((N_1 - N_0) / std::sqrt(size * 0.95 * 0.05 / 4));
    return boost::math::erfc(d / std::sqrt(2));
}

bool nist::check_discrete_fourier_transform(const utils::seq_bytes &bytes) {
    return nist::discrete_fourier_transform(bytes) >= alpha;
}

std::double_t nist::non_overlapping_template_matching(const utils::seq_bytes &bytes, const utils::seq_bytes &template_,
                                                      size_t N) {
    size_t n = bytes.size();
    size_t m = template_.size();
    size_t M = n / N;
    std::vector<utils::seq_bytes> blocks(N);
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = i * M; j < (i + 1) * M; ++j) {
            blocks[i].push_back(bytes[j]);
        }
    }
    std::vector<size_t> W(N, 0);
    for (size_t i = 0; i < N; ++i) {
        size_t index = 0;
        auto &current_block = blocks[i];
        while (index <= M - m) {
            bool match = true;
            size_t temp = index;
            for (size_t j = 0; j < m; ++j) {
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
    std::double_t pow2 = std::pow(2, m);
    std::double_t a = (M - m + 1) / pow2;
    std::double_t b = M * (1 / pow2 - (2 * m - 1) / (pow2 * pow2));
    std::double_t kappa = 0;
    for (size_t i = 0; i < N; ++i) {
        kappa += (W[i] - a) * (W[i] - a);
    }
    kappa /= b;
    return boost::math::gamma_q(static_cast<std::double_t>(N) / 2.0, kappa / 2.0);
}

bool nist::check_non_overlapping_template_matching(const utils::seq_bytes &bytes, const utils::seq_bytes &template_,
                                                   size_t N) {
    return nist::non_overlapping_template_matching(bytes, template_, N) >= alpha;
}

std::double_t nist::overlapping_template_matching(const utils::seq_bytes &bytes, const utils::seq_bytes &template_,
                                                  size_t M, size_t N, size_t K) {
    size_t n = bytes.size();
    size_t m = template_.size();
    std::vector<utils::seq_bytes> blocks(N);
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = i * M; j < (i + 1) * M; ++j) {
            blocks[i].push_back(bytes[j]);
        }
    }
    std::vector<size_t> W(N, 0);
    std::vector<size_t> v(6, 0);
    for (size_t i = 0; i < N; ++i) {
        size_t index = 0;
        auto &current_block = blocks[i];
        for (size_t index = 0; index <= M - m; index++) {
            bool match = true;
            size_t temp = index;
            for (size_t j = 0; j < m; ++j, temp++) {
                if (current_block[temp] != template_[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                W[i]++;
            }
        }
        if (W[i] <= 4) {
            v[W[i]]++;
        } else {
            v[K]++;
        }
    }
    std::double_t lambda = (M - m + 1) / std::pow(2, m);
    std::double_t theta = lambda / 2;
    std::vector<std::double_t> pi(K + 1);
    if (M == 10 && N == 5) {
        pi[0] = 0.324652;
        pi[1] = 0.182617;
        pi[2] = 0.142670;
        pi[3] = 0.106645;
        pi[4] = 0.077147;
        pi[5] = 0.166269;
    } else {
        pi[0] = std::exp(-theta);
        std::double_t a = std::exp(-2 * theta) * theta;
        std::double_t sum = pi[0];
        for (size_t i = 1; i < K; ++i) {
            std::double_t b = std::pow(2, i);
            pi[i] = a * boost::math::hypergeometric_1F1(std::double_t(i + 1), std::double_t(2), theta) / b;
            sum += pi[i];
        }
        pi[K] = 1 - sum;
    }
    std::double_t kappa = 0;
    for (size_t i = 0; i < K + 1; ++i) {
        kappa += (v[i] - N * pi[i]) * (v[i] - N * pi[i]) / (N * pi[i]);
    }
    return boost::math::gamma_q(K / 2.0, kappa / 2.0);
}

bool nist::check_overlapping_template_matching(const utils::seq_bytes &bytes, const utils::seq_bytes &template_,
                                               size_t M, size_t N, size_t K) {
    return nist::overlapping_template_matching(bytes, template_, M, N, K) >= alpha;
}

std::double_t nist::universal(const utils::seq_bytes &bytes) {
    std::vector<std::double_t> expected_value = {0,         0,         0,         0,         0,         0,
                                                 5.2177052, 6.1962507, 7.1836656, 8.1764248, 9.1723243, 10.170032,
                                                 11.168765, 12.168070, 13.167693, 14.167488, 15.167379};
    std::vector<std::double_t> variance = {0,     0,     0,     0,     0,     0,     2.954, 3.125, 3.238,
                                           3.311, 3.356, 3.384, 3.401, 3.410, 3.416, 3.419, 3.421};
    size_t n = bytes.size();
    size_t L = 5;
    if (n >= 387840) {
        L = 6;
    }
    if (n >= 904960) {
        L = 7;
    }
    if (n >= 2068480) {
        L = 8;
    }
    if (n >= 4654080) {
        L = 9;
    }
    if (n >= 10342400) {
        L = 10;
    }
    if (n >= 22753280) {
        L = 11;
    }
    if (n >= 49643520) {
        L = 12;
    }
    if (n >= 107560960) {
        L = 13;
    }
    if (n >= 231669760) {
        L = 14;
    }
    if (n >= 496435200) {
        L = 15;
    }
    if (n >= 1059061760) {
        L = 16;
    }
    size_t p = std::pow(2, L);
    size_t Q = 10 * p;
    if ((L < 6) || (L > 16)) {
        std::cout << "UNIVERSAL STATISTICAL TEST: ERROR : L IS OUT OF RANGE." << std::endl;
        return 0.0;
    }
    size_t K = n / L - Q;
    std::vector<size_t> T(p, 0);
#pragma omp parallel for
    for (size_t i = 1; i <= Q; ++i) {
        size_t dec_rep = 0;
        for (size_t j = 0; j < L; ++j) {
            dec_rep += bytes[(i - 1) * L + j] * std::pow(2, L - j - 1);
        }
        T[dec_rep] = i;
    }
    std::double_t sum = 0;
    for (size_t i = Q + 1; i <= Q + K; ++i) {
        size_t dec_rep = 0;
        for (size_t j = 0; j < L; ++j) {
            dec_rep += bytes[(i - 1) * L + j] * std::pow(2, L - j - 1);
        }
        sum += std::log(i - T[dec_rep]) / std::log(2);
        T[dec_rep] = i;
    }
    std::double_t f_n = sum / (std::double_t)K;
    std::double_t c = 0.7 - 0.8 / L + (4 + 32.0 / (std::double_t)L) * std::pow(K, -3.0 / (std::double_t)L) / 15;
    std::double_t sigma = c * std::sqrt(variance[L] / (std::double_t)K);
    return boost::math::erfc(std::abs((f_n - expected_value[L]) / (std::sqrt(2) * sigma)));
}

bool nist::check_universal(const utils::seq_bytes &bytes) {
    return nist::universal(bytes) >= alpha;
}

std::double_t nist::linear_complexity(const utils::seq_bytes &bytes, size_t M) {
    std::vector<std::double_t> pi = {0.01047, 0.03125, 0.12500, 0.50000, 0.25000, 0.06250, 0.020833};
    size_t n = bytes.size();
    size_t K = 6;
    size_t N = n / M;
    std::vector<size_t> v(K + 1, 0);
    utils::seq_bytes B_(M, 0);
    utils::seq_bytes C(M, 0);
    utils::seq_bytes P(M, 0);
    utils::seq_bytes T(M, 0);
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            B_[j] = 0;
            C[j] = 0;
            P[j] = 0;
            T[j] = 0;
        }
        C[0] = 1;
        B_[0] = 1;
        int L = 0;
        int m = -1;
        int N_ = 0;
        while (N_ < M) {
            int d = bytes[i * M + N_];
            for (size_t j = 1; j <= L; ++j) {
                d += C[j] * bytes[i * M + N_ - j];
            }
            d %= 2;
            if (d == 1) {
                for (size_t j = 0; j < M; ++j) {
                    T[j] = C[j];
                    P[j] = 0;
                }
                for (size_t j = 0; j < M; ++j) {
                    if (B_[j] == 1) {
                        P[j + N_ - m] = 1;
                    }
                }
                for (size_t j = 0; j < M; ++j) {
                    C[j] = (C[j] + P[j]) % 2;
                }
                if (L <= N_ / 2) {
                    L = N_ + 1 - L;
                    m = N_;
                    for (size_t j = 0; j < M; ++j) {
                        B_[j] = T[j];
                    }
                }
            }
            N_++;
        }
        int sign = (M + 1) % 2 == 0 ? -1 : 1;
        std::double_t mean = M / 2.0 + (9.0 + sign) / 36.0 - (M / 3.0 + 2.0 / 9.0) / std::pow(2, M);
        std::double_t T_ = sign * (L - mean) + 2.0 / 9.0;
        if (T_ <= -2.5) {
            v[0]++;
        } else if (T_ > -2.5 && T_ <= -1.5) {
            v[1]++;
        } else if (T_ > -1.5 && T_ <= -0.5) {
            v[2]++;
        } else if (T_ > -0.5 && T_ <= 0.5) {
            v[3]++;
        } else if (T_ > 0.5 && T_ <= 1.5) {
            v[4]++;
        } else if (T_ > 1.5 && T_ <= 2.5) {
            v[5]++;
        } else {
            v[6]++;
        }
    }
    std::double_t kappa = 0;
    for (size_t i = 0; i <= K; ++i) {
        kappa += std::pow(v[i] - N * pi[i], 2) / (N * pi[i]);
    }
    return boost::math::gamma_q(K / 2.0, kappa / 2.0);
}

bool nist::check_linear_complexity(const utils::seq_bytes &bytes, size_t M) {
    return nist::linear_complexity(bytes, M) >= alpha;
}

std::double_t psi(const utils::seq_bytes &bytes, size_t m) {
    if (m == 0) {
        return 0;
    }
    size_t n = bytes.size();

    std::vector<size_t> vi(std::pow(2, m + 1) - 1, 0);
#pragma omp parallel for
    for (size_t i = 0; i < n; ++i) {
        size_t k = 1;
        for (size_t j = 0; j < m; ++j) {
            if (bytes[(i + j) % n] == 0) {
                k *= 2;
            } else if (bytes[(i + j) % n] == 1) {
                k = 2 * k + 1;
            }
        }
        vi[k - 1]++;
    }

    std::double_t result = 0;
    int index = std::pow(2, m);
    for (int i = index - 1; i < index * 2 - 1; ++i) {
        result += std::pow(vi[i], 2);
    }
    return result * index / n - n;
}

std::pair<std::double_t, std::double_t> nist::serial_complexity(const utils::seq_bytes &bytes, size_t m) {
    std::double_t psi0 = psi(bytes, m);
    std::double_t psi1 = psi(bytes, m - 1);
    std::double_t psi2 = psi(bytes, m - 2);
    std::double_t del1 = psi0 - psi1;
    std::double_t del2 = psi0 - 2 * psi1 + psi2;
    std::double_t arg = std::pow(2, (int)m - 2);
    return {boost::math::gamma_q(arg, del1 / 2.0), boost::math::gamma_q(arg / 2, del2 / 2.0)};
}

bool nist::check_serial_complexity(const utils::seq_bytes &bytes, size_t m) {
    std::pair<std::double_t, std::double_t> p_value = nist::serial_complexity(bytes, m);
    return p_value.first >= alpha && p_value.second >= alpha;
}

std::double_t ap_en(const utils::seq_bytes &bytes, size_t block_size) {
    if (block_size == 0) {
        return 0;
    }
    size_t n = bytes.size();
    size_t pow_len = std::pow(2, block_size + 1) - 1;
    std::vector<size_t> vi(pow_len, 0);
    for (size_t i = 0; i < n; ++i) {
        int k = 1;
        for (size_t j = 0; j < block_size; ++j) {
            k <<= 1;
            if (bytes[(i + j) % n] == 1) {
                k++;
            }
        }
        vi[k - 1]++;
    }
    std::double_t sum = 0;
    size_t right_bound = std::pow(2, block_size);
    size_t index = right_bound - 1;
    for (size_t i = 0; i < right_bound; ++i, index++) {
        if (vi[index] > 0) {
            sum += vi[index] * std::log(vi[index] * 1.0 / n);
        }
    }
    return sum / n;
}

std::double_t nist::approximate_entropy(const utils::seq_bytes &bytes, size_t m) {
    size_t n = bytes.size();
    std::double_t psi0 = ap_en(bytes, m);
    std::double_t psi1 = ap_en(bytes, m + 1);
    std::double_t ApEn = psi0 - psi1;
    std::double_t kappa = 2 * n * (std::log(2) - ApEn);
    return boost::math::gamma_q(std::pow(2, m - 1), kappa / 2.0);
}

bool nist::check_approximate_entropy(const utils::seq_bytes &bytes, size_t m) {
    return nist::approximate_entropy(bytes, m) >= alpha;
}

std::vector<int> partial_sums(const std::vector<short> &x,
                              nist::CumulativeSumsMode mode = nist::CumulativeSumsMode::Forward) {
    size_t n = x.size();
    std::vector<int> S(n + 1, 0);
    if (mode == nist::CumulativeSumsMode::Forward) {
        for (size_t i = 1; i <= n; ++i) {
            S[i] = S[i - 1] + x[i - 1];
        }
    } else {
        for (size_t i = 1; i <= n; ++i) {
            S[i] = S[i - 1] + x[n - i];
        }
    }
    return S;
}

std::double_t nist::cumulative_sums(const utils::seq_bytes &bytes, nist::CumulativeSumsMode mode) {
    size_t n = bytes.size();
    std::vector<short> x = normalized(bytes);
    std::vector<int> S = partial_sums(x, mode);
    int z =
        std::abs(*std::max_element(S.begin(), S.end(), [](int lhs, int rhs) { return std::abs(lhs) < std::abs(rhs); }));
    std::double_t sum1 = 0;
    int left = (-(int)n / z + 1) / 4;
    int right = (n / z - 1) / 4;
    for (int i = left; i <= right; ++i) {
        sum1 += boost::math::cdf(boost::math::normal_distribution<std::double_t>(), ((4 * i + 1) * z) / std::sqrt(n)) -
                boost::math::cdf(boost::math::normal_distribution<std::double_t>(), ((4 * i - 1) * z) / std::sqrt(n));
    }

    std::double_t sum2 = 0;
    left = (-(int)n / z - 3) / 4;
    for (int i = left; i <= right; ++i) {
        sum2 += boost::math::cdf(boost::math::normal_distribution<std::double_t>(), ((4 * i + 3) * z) / std::sqrt(n)) -
                boost::math::cdf(boost::math::normal_distribution<std::double_t>(), ((4 * i + 1) * z) / std::sqrt(n));
    }
    return 1.0 - sum1 + sum2;
}

bool nist::check_cumulative_sums(const utils::seq_bytes &bytes, nist::CumulativeSumsMode mode) {
    return nist::cumulative_sums(bytes, mode) >= alpha;
}

std::vector<std::double_t> nist::random_excursions(const utils::seq_bytes &bytes) {
    std::vector<int> state_x = {-4, -3, -2, -1, 1, 2, 3, 4};
    constexpr size_t count_state = 8;
    std::vector<std::vector<std::double_t>> pi = {
        {0.0000000000, 0.00000000000, 0.00000000000, 0.00000000000, 0.00000000000, 0.0000000000},
        {0.5000000000, 0.25000000000, 0.12500000000, 0.06250000000, 0.03125000000, 0.0312500000},
        {0.7500000000, 0.06250000000, 0.04687500000, 0.03515625000, 0.02636718750, 0.0791015625},
        {0.8333333333, 0.02777777778, 0.02314814815, 0.01929012346, 0.01607510288, 0.0803755143},
        {0.8750000000, 0.01562500000, 0.01367187500, 0.01196289063, 0.01046752930, 0.0732727051}};
    std::vector<short> x = normalized(bytes);
    std::vector<int> S = partial_sums(x);
    std::vector<int> S_ = S;
    S_.push_back(0);
    size_t J = std::count_if(std::next(S_.begin()), S_.end(), [](int element) { return element == 0; });
    std::vector<std::vector<int>> cycles(J);
    size_t index = 0;
    cycles[index].push_back(0);
    for (size_t i = 1; i < S_.size(); ++i) {
        cycles[index].push_back(S_[i]);
        if (S_[i] == 0) {
            index++;
            if (index < J) {
                cycles[index].push_back(S_[i]);
            }
        }
    }
    std::vector<size_t> counter(count_state, 0);
    std::vector<std::vector<size_t>> vi(6, std::vector<size_t>(8, 0));
    for (const auto &cycle : cycles) {
        for (size_t i = 0; i < count_state; ++i) {
            counter[i] = 0;
        }
        for (size_t i = 0; i < count_state; ++i) {
            int elem = state_x[i];
            counter[i] = std::count_if(cycle.begin(), cycle.end(), [&elem](int element) { return element == elem; });
        }
        for (size_t i = 0; i < count_state; ++i) {
            if (counter[i] >= 0 && counter[i] <= 4) {
                vi[counter[i]][i]++;
            } else {
                vi[5][i]++;
            }
        }
    }
    std::vector<std::double_t> p_values(count_state, 0);
#pragma omp parallel for
    for (size_t i = 0; i < count_state; ++i) {
        int x = state_x[i];
        std::double_t sum = 0;
        for (size_t j = 0; j < 6; ++j) {
            sum += (std::pow(vi[j][i] - J * pi[(int)std::abs(x)][j], 2) / (J * pi[(int)std::abs(x)][j]));
        }
        p_values[i] = boost::math::gamma_q(2.5, sum / 2.0);
    }
    return p_values;
}

std::vector<bool> nist::check_random_excursions(const utils::seq_bytes &bytes) {
    std::vector<std::double_t> p_values = nist::random_excursions(bytes);
    std::vector<bool> results(p_values.size(), false);
    for (size_t i = 0; i < p_values.size(); ++i) {
        results[i] = p_values[i] >= alpha;
    }
    return results;
}

std::vector<std::double_t> nist::random_excursions_variant(const utils::seq_bytes &bytes) {
    std::vector<int> state_x = {-9, -8, -7, -6, -5, -4, -3, -2, -1, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    constexpr size_t count_state = 18;
    std::vector<short> x = normalized(bytes);
    std::vector<int> S = partial_sums(x);
    std::vector<int> S_ = S;
    S_.push_back(0);
    size_t J = std::count_if(std::next(S_.begin()), S_.end(), [](int element) { return element == 0; });

    std::vector<std::double_t> p_values(count_state, 0);
#pragma omp parallel for
    for (size_t i = 0; i < count_state; ++i) {
        int x = state_x[i];
        std::double_t count = std::count_if(S_.begin(), S_.end(), [&x](int element) { return element == x; });
        p_values[i] = boost::math::erfc(std::abs(count - J) / (std::sqrt(2 * J * (4 * std::abs(x) - 2))));
    }

    return p_values;
}

std::vector<bool> nist::check_random_excursions_variant(const utils::seq_bytes &bytes) {
    std::vector<std::double_t> p_values = nist::random_excursions_variant(bytes);
    std::vector<bool> results(p_values.size(), false);
    for (size_t i = 0; i < p_values.size(); ++i) {
        results[i] = p_values[i] >= alpha;
    }
    return results;
}
