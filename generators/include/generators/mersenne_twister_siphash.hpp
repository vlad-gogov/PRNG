#pragma once

#include <array>

#include "generators/generator.hpp"
#include "generators/mersenne_twister.hpp"
#include "hash_functions/siphash.hpp"

template <size_t W, size_t N, size_t M, size_t R, uint32_t A, size_t U, uint32_t D, size_t S, uint32_t B, size_t T,
          uint32_t C, size_t L, uint32_t F>
class MersenneTwisterEngineSiphash : public Generator<uint32_t> {
    MersenneTwisterEngine<uint32_t, W, N, M, R, A, U, D, S, B, T, C, L, F> mt_gen;
    static constexpr uint32_t default_seed = 5489u;
    siphash::Key key = {1234567890, 987654321};

  public:
    MersenneTwisterEngineSiphash(const uint32_t seed = default_seed) : mt_gen(default_seed) {
    }

    uint32_t operator()() noexcept override {
        uint32_t y = mt_gen();
        std::vector<std::uint8_t> bytes(4, 0);
        for (size_t i = 0; i < 4; ++i) {
            bytes[i] = (y >> (i * 8)) & 0xFF;
        }
        std::uint64_t hash = siphash::siphash24(bytes.data(), 4, &key);
        return static_cast<uint32_t>(hash & 0xFFFFFFFF);
    }

    uint32_t min() const override {
        return mt_gen.min();
    }

    uint32_t max() const override {
        return mt_gen.max();
    }

    void seed(const uint32_t seed) override {
        mt_gen = MersenneTwisterEngine<uint32_t, W, N, M, R, A, U, D, S, B, T, C, L, F>(seed);
    }

    void discard(const std::uint64_t z) override {
        for (size_t i = 0; i < z; ++i) {
            this->operator()();
        }
    }
};

using MT19937SIPHASH = MersenneTwisterEngineSiphash<32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL,
                                                    15, 0xefc60000UL, 18, 1812433253UL>;
