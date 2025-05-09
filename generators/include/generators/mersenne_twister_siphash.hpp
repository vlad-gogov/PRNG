#pragma once

#include <array>

#include "generators/generator.hpp"
#include "generators/mersenne_twister.hpp"
#include "hash_functions/siphash.hpp"

template <typename UIntType, size_t W, size_t N, size_t M, size_t R, UIntType A, size_t U, UIntType D, size_t S,
          UIntType B, size_t T, UIntType C, size_t L, UIntType F>
class MersenneTwisterEngineSiphash : public Generator<UIntType> {
    MersenneTwisterEngine<UIntType, W, N, M, R, A, U, D, S, B, T, C, L, F> mt_gen;
    static constexpr UIntType default_seed = 5489u;
    siphash::Key key = {1234567890, 987654321};

  public:
    MersenneTwisterEngineSiphash(const UIntType seed = default_seed) : mt_gen(seed) {
    }

    UIntType operator()() noexcept override {
        constexpr size_t bytes = sizeof(UIntType);
        UIntType y = mt_gen();
        std::vector<std::uint8_t> new_bytes(bytes, 0);
        for (size_t i = 0; i < bytes; ++i) {
            new_bytes[i] = (y >> (i * 8)) & 0xFF;
        }
        std::uint64_t hash = siphash::siphash24(new_bytes.data(), bytes, &key);
        return static_cast<UIntType>(hash);
    }

    UIntType min() const override {
        return mt_gen.min();
    }

    UIntType max() const override {
        return mt_gen.max();
    }

    void seed(const UIntType seed) override {
        mt_gen = MersenneTwisterEngine<UIntType, W, N, M, R, A, U, D, S, B, T, C, L, F>(seed);
    }

    void discard(const std::uint64_t z) override {
        for (size_t i = 0; i < z; ++i) {
            this->operator()();
        }
    }
};

using MT19937SIPHASH = MersenneTwisterEngineSiphash<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                    0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL>;

template <typename UIntType, size_t W, size_t N, size_t M0, size_t M1, size_t M2, size_t R, UIntType A, size_t U,
          size_t S, UIntType B, size_t T, UIntType C, size_t L, UIntType F>
class MersenneTwisterEngineSiphash64 : public Generator<UIntType> {
    MersenneTwisterEngine64<UIntType, W, N, M0, M1, M2, R, A, U, S, B, T, C, L, F> mt_gen;
    static constexpr UIntType default_seed = 5489u;
    siphash::Key key = {1234567890, 987654321};

  public:
    MersenneTwisterEngineSiphash64(const UIntType seed = default_seed) : mt_gen(seed) {
    }

    UIntType operator()() noexcept override {
        constexpr size_t bytes = sizeof(UIntType);
        UIntType y = mt_gen();
        std::vector<std::uint8_t> new_bytes(bytes, 0);
        for (size_t i = 0; i < bytes; ++i) {
            new_bytes[i] = (y >> (i * 8)) & 0xFF;
        }
        std::uint64_t hash = siphash::siphash24(new_bytes.data(), bytes, &key);
        return static_cast<UIntType>(hash);
    }

    UIntType min() const override {
        return mt_gen.min();
    }

    UIntType max() const override {
        return mt_gen.max();
    }

    void seed(const UIntType seed) override {
        mt_gen = MersenneTwisterEngine64<UIntType, W, N, M0, M1, M2, R, A, U, S, B, T, C, L, F>(seed);
    }

    void discard(const std::uint64_t z) override {
        for (size_t i = 0; i < z; ++i) {
            this->operator()();
        }
    }
};
using MT19937SIPHASH_64 =
    MersenneTwisterEngineSiphash64<uint64_t, 64, 312, 63, 151, 224, 31, 0xB3815B624FC82E2FULL, 26, 17,
                                   0x656BEDFFD9A40000ULL, 33, 0xFFFAAFFE00000000ULL, 39, 6364136223846793005ULL>;
