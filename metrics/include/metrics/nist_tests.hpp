#include <iostream>

#include "utils.hpp"

namespace nist {

double frequency_test(const utils::seq_bytes &bytes);
bool check_frequency_test(const utils::seq_bytes &bytes);

double frequency_block_test(const utils::seq_bytes &bytes, size_t m);
bool check_frequency_block_test(const utils::seq_bytes &bytes, size_t m);

double runs_test(const utils::seq_bytes &bytes);
bool check_runs_test(const utils::seq_bytes &bytes);

double longest_run_of_ones(const utils::seq_bytes &bytes);
bool check_longest_run_of_ones(const utils::seq_bytes &bytes);

double binary_matrix_rank(const utils::seq_bytes &bytes, size_t M, size_t Q);
bool check_binary_matrix_rank(const utils::seq_bytes &bytes, size_t M, size_t Q);

double discrete_fourier_transform(const utils::seq_bytes &bytes);
bool check_discrete_fourier_transform(const utils::seq_bytes &bytes);

double non_overlapping_template_matching(const utils::seq_bytes &bytes, const utils::seq_bytes &template_,
                                         size_t N = 8);
bool check_non_overlapping_template_matching(const utils::seq_bytes &bytes, const utils::seq_bytes &template_,
                                             size_t N = 8);

double overlapping_template_matching(const utils::seq_bytes &bytes, const utils::seq_bytes &template_, size_t M = 1032,
                                     size_t N = 968, size_t K = 5, bool test = false);
bool check_overlapping_template_matching(const utils::seq_bytes &bytes, const utils::seq_bytes &template_,
                                         size_t M = 1032, size_t N = 968, size_t K = 5, bool test = false);

double universal(const utils::seq_bytes &bytes);
bool check_universal(const utils::seq_bytes &bytes);

double linear_complexity(const utils::seq_bytes &bytes, size_t M);
bool check_linear_complexity(const utils::seq_bytes &bytes, size_t M);

std::pair<double, double> serial_complexity(const utils::seq_bytes &bytes, size_t m);
bool check_serial_complexity(const utils::seq_bytes &bytes, size_t m);

double approximate_entropy(const utils::seq_bytes &bytes, size_t m);
bool check_approximate_entropy(const utils::seq_bytes &bytes, size_t m);

enum CumulativeSumsMode {
    Forward,
    Reverse,
};

double cumulative_sums(const utils::seq_bytes &bytes, CumulativeSumsMode mode);
bool check_cumulative_sums(const utils::seq_bytes &bytes, CumulativeSumsMode mode);

std::vector<double> random_excursions(const utils::seq_bytes &bytes);
std::vector<bool> check_random_excursions(const utils::seq_bytes &bytes);

std::vector<double> random_excursions_variant(const utils::seq_bytes &bytes, bool check = true);
std::vector<bool> check_random_excursions_variant(const utils::seq_bytes &bytes, bool check = true);

} // namespace nist
