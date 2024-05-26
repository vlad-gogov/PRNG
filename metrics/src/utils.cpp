#include "metrics/utils.hpp"

#include <algorithm>
#include <cmath>
#include <complex>
#include <filesystem>
#include <fstream>

namespace utils {

size_t get_max_run(const seq_bytes &seq, size_t left_border, size_t right_border) {
    if (right_border == 0U) {
        right_border = seq.size();
    }
    size_t max_run = 0U;
    size_t prev_index = left_border;
    bool is_seq = false;
    for (size_t i = left_border; i < right_border; ++i) {
        bool curr = seq[i];
        if (!is_seq && curr) {
            prev_index = i;
            is_seq = true;
        } else if (!curr && is_seq) {
            max_run = std::max(max_run, i - prev_index);
            is_seq = false;
        }
    }
    if (is_seq) {
        max_run = std::max(max_run, right_border - prev_index);
    }
    return max_run;
}

seq_bytes read_bits_from_exponent(size_t count) {
    std::string path = std::getenv("PATH_TO_DIGIT_EXPONENT");
    std::cout << path << std::endl;
    std::ifstream e_file(path, std::ios::binary | std::ios::in);
    if (count == 0) {
        e_file.seekg(0, std::ios::end);
        count = e_file.tellg();
        e_file.seekg(0, std::ios::beg);
    }
    seq_bytes bytes(count);
    size_t index_data = 0;
    if (e_file.is_open()) {
        for (size_t i = 0; i < count; i++) {
            e_file >> bytes[i];
        }
    } else {
        throw std::runtime_error("Can't open file: " + path);
    }
    e_file.close();
    return bytes;
}

} // namespace utils
