#include <iostream>

#include "utils.hpp"

namespace nist {

double frequency_test(const utils::seq_bytes &bytes);
bool check_frequency_test(const utils::seq_bytes &bytes);

double frequency_block_test(const utils::seq_bytes &bytes, size_t count_part);
bool check_frequency_block_test(const utils::seq_bytes &bytes, size_t count_part);

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

double overlapping_template_matching(const utils::seq_bytes &bytes, const utils::seq_bytes &template_, size_t N = 8,
                                     size_t K = 5);
bool check_overlapping_template_matching(const utils::seq_bytes &bytes, const utils::seq_bytes &template_, size_t N = 8,
                                         size_t K = 5);

double universal(const utils::seq_bytes &bytes, size_t L, size_t Q);
bool check_universal(const utils::seq_bytes &bytes, size_t L, size_t Q);

} // namespace nist
