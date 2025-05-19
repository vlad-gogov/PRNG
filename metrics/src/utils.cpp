#include "metrics/utils.hpp"

#include <algorithm>
#include <boost/math/special_functions/gamma.hpp>
#include <cmath>
#include <complex>
#include <filesystem>

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

double chi_square(std::vector<double> trial_vector, std::vector<double> expected_vector, int degrees_of_freedom) {
    double chi_sq = 0.0;
    for (size_t i = 0; i < degrees_of_freedom; i++) {
        double part_chi_sq =
            (trial_vector[i] - expected_vector[i]) * (trial_vector[i] - expected_vector[i]) / expected_vector[i];
        chi_sq += part_chi_sq;
    }
    return chi_sq;
}

double p_value(int degrees_of_freedom, double chi_square) {
    return boost::math::gamma_q((double)(degrees_of_freedom) / 2.0, chi_square / 2.0);
}

double poissonian(int k, double lambda) {
    return std::pow(lambda, k) * std::exp(-lambda) / boost::math::factorial<double>(k);
}

std::vector<double> bits_to_doubles(const seq_bytes &bytes, int num_floats) {
    // Converts bits to doubles [0;1)

    std::vector<uint64_t> uintValues = bits_to_vector_uint<uint64_t>(bytes, num_floats);

    std::vector<double> doubleVector(num_floats);
    const double maxUInt64 = static_cast<double>(UINT64_MAX);

    for (int i = 0; i < num_floats; ++i) {
        doubleVector[i] = static_cast<double>(uintValues[i]) / maxUInt64;
    }

    return doubleVector;
}

std::vector<double> random_doubles(int num_doubles) {
    std::vector<double> result_doubles(num_doubles);
    for (size_t i = 0; i < num_doubles; i++) {
        double x = ((double)std::rand() / (RAND_MAX));
        result_doubles.push_back(x);
    }
    return result_doubles;
}

double kstest(std::vector<double> p_values) {
    // Calculates p_value for vector of p_values.
    int count = p_values.size();
    if (count < 1)
        return -1;
    if (count == 1)
        return p_values[0];
    double d_max = 0.0;
    for (size_t i = 0; i < count; i++) {
        double y = (double)i / (count + 1.0);
        double d1 = p_values[i - 1] - y;
        double d2 = std::abs(1.0 / (count + 1.0) - d1);
        d1 = std::abs(d1);
        double d = std::max(d1, d2);
        if (d > d_max)
            d_max = d;
    }
    double s = d_max * d_max * count;
    return 2.0 * std::exp(-(2.000071 + .331 / std::sqrt(count) + 1.409 / count) * s);
}

int kperm(const std::vector<int> &v) {
    // This function computes a unique permutation index for a given array of five integers.
    // This index uniquely represents the ordering of the integers in a factoradic numbering system, effectively mapping
    // the permutation to a specific number.
    std::vector<int> w = v;
    int pindex = 0, uret, tmp;

    // Work on a copy of v, not v itself in case we are using overlapping 5-patterns.
    for (int i = 4; i > 0; --i) {
        int max = w[0];
        int k = 0;
        for (int j = 1; j <= i; ++j) {
            if (max <= w[j]) {
                max = w[j];
                k = j;
            }
        }
        pindex = (i + 1) * pindex + k;
        std::swap(w[i], w[k]);
    }

    uret = pindex;

    return uret;
}

void save_string_to_file(const std::string &path, const std::string &str, bool append) {
    std::ofstream file;
    if (append) {
        file.open(path, std::ios::app);
    } else {
        file.open(path, std::ios::out);
    }
    file << str;
    file.close();
}

} // namespace utils
