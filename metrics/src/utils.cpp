#include "utils.hpp"

#include <algorithm>

size_t utils::get_max_run(const seq_bytes &seq, size_t left_border, size_t right_border) {
    if (right_border == 0U) {
        right_border = seq.size();
    }
    size_t max_run = 0U;
    size_t prev_index = left_border;
    bool isSeq = false;
    for (size_t i = left_border; i < right_border; ++i) {
        bool curr = seq[i];
        if (!isSeq && curr) {
            prev_index = i;
            isSeq = true;
        } else if (!curr && isSeq) {
            max_run = std::max(max_run, i - prev_index);
            isSeq = false;
        }
    }
    if (isSeq) {
        max_run = std::max(max_run, right_border - prev_index);
    }
    return max_run;
}
