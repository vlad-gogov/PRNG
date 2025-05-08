#include "metrics/utils.hpp"

#include <algorithm>
#include <boost/math/special_functions/gamma.hpp>
#include <cmath>
#include <complex>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <chrono>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdint>
#include <cstring>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

namespace utils {

// Global CURL initialization
class CurlInitializer {
public:
    CurlInitializer() { curl_global_init(CURL_GLOBAL_ALL); }
    ~CurlInitializer() { curl_global_cleanup(); }
};

static CurlInitializer curl_init;

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

uint64_t generate_seed_from_mouse() {
    // Set terminal to raw mode
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Set stdin to non-blocking
    int oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    // Enable mouse tracking (1003: all motion)
    std::cout << "\033[?1003h\033[?1006h" << std::flush;
    std::cout << "\nMove your mouse in this terminal window...\n";

    std::vector<uint8_t> entropy;
    auto start = std::chrono::steady_clock::now();
    const int min_events = 64;
    int events = 0;
    while (events < min_events) {
        char buf[32];
        ssize_t n = read(STDIN_FILENO, buf, sizeof(buf));
        if (n > 0) {
            for (ssize_t i = 0; i < n; ++i) {
                // Look for SGR mouse event: ESC [ < b ; x ; y M
                if (buf[i] == '\033' && i + 1 < n && buf[i+1] == '[') {
                    ssize_t j = i + 2;
                    if (j < n && buf[j] == '<') {
                        // Try to parse the event
                        int b = 0, x = 0, y = 0;
                        char type = 0;
                        int consumed = 0;
                        if (sscanf(&buf[j], "<%d;%d;%d%c", &b, &x, &y, &type) == 4) {
                            auto now = std::chrono::steady_clock::now();
                            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
                            entropy.push_back((uint8_t)(x ^ y ^ b ^ ms));
                            events++;
                            std::cout << "\rCollected " << events << "/" << min_events << " events..." << std::flush;
                        }
                    }
                }
            }
        }
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(now - start).count() > 10) break;
        usleep(1000); // 1ms
    }

    // Disable mouse tracking
    std::cout << "\033[?1003l\033[?1006l" << std::flush;
    // Restore terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    // Simple hash: FNV-1a
    uint64_t hash = 14695981039346656037ull;
    for (uint8_t b : entropy) {
        hash ^= b;
        hash *= 1099511628211ull;
    }
    return hash;
}

uint64_t generate_seed_from_time() {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count());
}

uint64_t generate_seed_from_qrng() {
    
    // Create a temporary file for the response
    std::string temp_file = "/tmp/qrng_response.json";
    std::string curl_cmd = "curl -s -m 5 'https://lfdr.de/qrng_api/qrng?length=8&format=HEX' > " + temp_file;
    
    // Execute curl command
    int result = system(curl_cmd.c_str());
    if (result != 0) {
        throw std::runtime_error("Failed to execute curl command");
    }

    // Read the response from the temporary file
    std::ifstream file(temp_file);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open response file");
    }

    std::string response;
    std::getline(file, response);
    file.close();

    // Clean up the temporary file
    std::remove(temp_file.c_str());

    if (response.empty()) {
        throw std::runtime_error("Empty response from QRNG API");
    }

    try {
        auto json = nlohmann::json::parse(response);
        
        if (!json.contains("qrn")) {
            throw std::runtime_error("QRNG response missing 'qrn' field");
        }
        
        std::string hex = json["qrn"].get<std::string>();
        
        if (hex.empty()) {
            throw std::runtime_error("Empty QRNG data received");
        }
        
        uint64_t result = std::stoull(hex, nullptr, 16);
        return result;
        
    } catch (const nlohmann::json::exception& e) {
        throw std::runtime_error(std::string("JSON parse error: ") + e.what());
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Processing error: ") + e.what());
    }
}

} // namespace utils
