#include <iostream>
#include <string>
#include <unordered_map>

#include "generators/linear_congruential_generator.hpp"
#include "generators/mersenne_twister.hpp"
#include "generators/mersenne_twister_rotr.hpp"
#include "generators/mersenne_twister_sbox.hpp"
#include "generators/mersenne_twister_sbox_and_rotr.hpp"
#include "generators/mersenne_twister_simd.hpp"
#include "generators/mersenne_twister_siphash.hpp"

#include "utils.hpp"

enum class GeneratorType {
    MINSTD_RAND0,
    LINE_LCG,
    MT19937,
    MT19937_64,
    MT19937SBOX,
    MT19937Rotr3,
    MT19937Rotr14,
    MT19937Rotr24,
    MT19937Rotr26,
    MT19937Rotr31,
    MT19937SBOXRotr31,
    MT19937SIPHASH
};

static std::unordered_map<std::string, GeneratorType> const table = {
    {"MINSTD_RAND0", GeneratorType::MINSTD_RAND0},
    {"LINE_LCG", GeneratorType::LINE_LCG},
    {"MT19937", GeneratorType::MT19937},
    {"MT19937_64", GeneratorType::MT19937_64},
    {"MT19937SBOX", GeneratorType::MT19937SBOX},
    {"MT19937Rotr3", GeneratorType::MT19937Rotr3},
    {"MT19937Rotr14", GeneratorType::MT19937Rotr14},
    {"MT19937Rotr24", GeneratorType::MT19937Rotr24},
    {"MT19937Rotr26", GeneratorType::MT19937Rotr26},
    {"MT19937Rotr31", GeneratorType::MT19937Rotr31},
    {"MT19937SBOXRotr31", GeneratorType::MT19937SBOXRotr31},
    {"MT19937SIPHASH", GeneratorType::MT19937SIPHASH}};

int main(int argc, char *argv[]) {
    if (argc < 4 || argc > 6) {
        std::cerr << "Wrong count args" << std::endl;
        return 1;
    }

    std::string path_to_file = argv[1];
    std::string generator_name = argv[2];
    size_t data_size = std::atoll(argv[3]);
    uint32_t seed = std::atoll(argv[4]);

    std::cout << path_to_file << " " << generator_name << " " << data_size << " " << seed << std::endl;

    GeneratorType generator_type;
    auto it = table.find(generator_name);
    if (it != table.end()) {
        generator_type = it->second;
    } else {
        std::cerr << "Wrong generator name" << std::endl;
        return 1;
    }

    switch (generator_type) {
    case GeneratorType::MINSTD_RAND0:
        save_numbers_to_file<MINSTD_RAND0>(path_to_file, data_size, seed);
        break;
    case GeneratorType::LINE_LCG:
        save_numbers_to_file<LINE_LCG>(path_to_file, data_size, seed);
        break;
    case GeneratorType::MT19937:
        save_numbers_to_file<MT19937>(path_to_file, data_size, seed);
        break;
    case GeneratorType::MT19937_64:
        save_numbers_to_file<MT19937_64>(path_to_file, data_size, seed);
        break;
    case GeneratorType::MT19937SBOX:
        save_numbers_to_file<MT19937SBOX>(path_to_file, data_size, seed);
        break;
    case GeneratorType::MT19937Rotr3:
        save_numbers_to_file<MT19937Rotr3>(path_to_file, data_size, seed);
        break;
    case GeneratorType::MT19937Rotr14:
        save_numbers_to_file<MT19937Rotr14>(path_to_file, data_size, seed);
        break;
    case GeneratorType::MT19937Rotr24:
        save_numbers_to_file<MT19937Rotr24>(path_to_file, data_size, seed);
        break;
    case GeneratorType::MT19937Rotr26:
        save_numbers_to_file<MT19937Rotr26>(path_to_file, data_size, seed);
        break;
    case GeneratorType::MT19937Rotr31:
        save_numbers_to_file<MT19937Rotr31>(path_to_file, data_size, seed);
        break;
    case GeneratorType::MT19937SBOXRotr31:
        save_numbers_to_file<MT19937SBOXRotr31>(path_to_file, data_size, seed);
        break;
    case GeneratorType::MT19937SIPHASH:
        save_numbers_to_file<MT19937SIPHASH>(path_to_file, data_size, seed);
        break;
    default:
        std::cerr << "Wrong generator type" << std::endl;
        return 1;
    }

    return 0;
}
