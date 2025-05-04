#pragma once

#include <fstream>

template <typename Generator>
void save_numbers_to_file(const std::string &path, const size_t count_number,
                          const typename Generator::result_type seed) {
    Generator generator(seed);
    std::ofstream file;
    file.open(path, std::ios::out);
    for (size_t i = 0; i < count_number; ++i) {
        file << generator() << "\n";
    }
    file.close();
}
