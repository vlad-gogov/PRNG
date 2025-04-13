#include <iostream>

#include "utils.hpp"

namespace diehard {

double runs_test(const utils::seq_bytes &bytes);

double matrix_rank_prob(int M, int Q, int rank);

double matrix_test(const utils::seq_bytes &bytes, int rows, int cols, int iterations);

double birthdays_test(const utils::seq_bytes &bytes, int days_bits, int num_bdays, int tsamples);

double minimum_distance_test(const utils::seq_bytes &bytes, int n_dims, int num_coordinates, int num_samples);

double overlapping_permutations_test(const utils::seq_bytes &bytes, int num_samples);

double base_5_word_chi_sq(const utils::seq_bytes &bytes, int num_samples, int word_length);
double monkey_test(const utils::seq_bytes &bytes, int num_samples);

double squeeze_test(const utils::seq_bytes &bytes, int num_samples);

double sums_test(const utils::seq_bytes &bytes, int num_samples);

double craps_test(const utils::seq_bytes &bytes, int num_games);

} // namespace diehard
