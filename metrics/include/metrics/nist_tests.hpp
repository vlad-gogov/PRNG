#include <iostream>

#include "utils.hpp"

namespace nist {

double frequency_test(const utils::seq_bytes &bytes);
bool check_frequency_test(const utils::seq_bytes &bytes);

double frequency_block_test(const utils::seq_bytes &bytes, size_t count_part);
bool check_frequency_block_test(const utils::seq_bytes &bytes, size_t count_part);

double runs_test(const utils::seq_bytes &bytes);
bool check_runs_test(const utils::seq_bytes &bytes);

} // namespace nist