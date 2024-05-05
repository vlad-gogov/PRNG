#include <iostream>

#include "utils.hpp"

namespace diehard {

double runs_test(const utils::seq_bytes &bytes);

double matrix_rank_prob(int M, int Q, int rank);

double matrix_test(const utils::seq_bytes &bytes, int rows, int cols, int iterations);

double birthdays_test(const utils::seq_bytes &bytes, int days_bits, int num_bdays, int tsamples);

} // namespace diehard
