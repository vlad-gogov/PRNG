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

seq_bytes read_bytes_from_file(const std::string path, size_t count) {
    seq_bytes bytes(count);
    std::stringstream path_stream;
    path_stream << std::string(std::filesystem::current_path().parent_path().c_str());
    path_stream << "/../";
    path_stream << path;
    std::ifstream e_file(path_stream.str(), std::ios::binary | std::ios::ate | std::ios::in);
    size_t index = 0;
    size_t index_data = 0;
    if (e_file.is_open()) {
        auto size = e_file.tellg();
        std::string str(size, '\0');
        e_file.seekg(0);
        if (e_file.read(&str[0], size)) {
            for (size_t i = 0; index < count && i < str.size(); i++) {
                if (str[i] == '1') {
                    bytes[index++] = 1;
                } else if (str[i] == '0') {
                    bytes[index++] = 0;
                }
            }
        }
    } else {
        throw std::runtime_error("Can't open file: " + path);
    }
    e_file.close();
    return bytes;
}

} // namespace utils