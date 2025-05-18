#pragma once

#include <array>
#include <bit>
#include <vector>

#include "generators/generator.hpp"
#include "generators/mersenne_twister_sbox.hpp"

template <typename UIntType, size_t W, size_t N, size_t M, size_t R, UIntType A, size_t U, UIntType D, size_t S,
          UIntType B, size_t T, UIntType C, size_t L, UIntType F, UIntType shift>
class MersenneTwisterEngineSBOXRotr : public Generator<uint32_t> {
    MersenneTwisterEngine<UIntType, W, N, M, R, A, U, D, S, B, T, C, L, F> mt_gen;
    static constexpr UIntType default_seed = 5489u;

  public:
    MersenneTwisterEngineSBOXRotr(const UIntType seed = default_seed) : mt_gen(seed) {
    }

    UIntType operator()() noexcept override {
        constexpr size_t bytes = sizeof(UIntType);
        UIntType raw_val = mt_gen();
        UIntType result = 0u;
        for (size_t i = 0; i < bytes; ++i) {
            result |= (AES_SBOX[(raw_val >> (i * 8)) & 0xFF]) << ((bytes - i - 1) * 8);
        }
        result = std::rotr(result, shift);
        return result;
    }

    UIntType min() const override {
        return mt_gen.min();
    }

    UIntType max() const override {
        return mt_gen.max();
    }

    void seed(const UIntType seed) override {
        mt_gen = MersenneTwisterEngine<uint32_t, W, N, M, R, A, U, D, S, B, T, C, L, F>(seed);
    }

    void discard(const std::uint64_t z) override {
        for (size_t i = 0; i < z; ++i) {
            this->operator()();
        }
    }
};

using MT19937SBOXRotr1 = MersenneTwisterEngineSBOXRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                       0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 1>;
using MT19937SBOXRotr2 = MersenneTwisterEngineSBOXRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                       0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 2>;
using MT19937SBOXRotr3 = MersenneTwisterEngineSBOXRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                       0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 3>;
using MT19937SBOXRotr4 = MersenneTwisterEngineSBOXRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                       0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 4>;
using MT19937SBOXRotr5 = MersenneTwisterEngineSBOXRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                       0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 5>;
using MT19937SBOXRotr6 = MersenneTwisterEngineSBOXRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                       0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 6>;
using MT19937SBOXRotr7 = MersenneTwisterEngineSBOXRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                       0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 7>;
using MT19937SBOXRotr8 = MersenneTwisterEngineSBOXRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                       0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 8>;
using MT19937SBOXRotr9 = MersenneTwisterEngineSBOXRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                       0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 9>;
using MT19937SBOXRotr10 = MersenneTwisterEngineSBOXRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                        0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 10>;
using MT19937SBOXRotr11 = MersenneTwisterEngineSBOXRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                        0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 11>;
using MT19937SBOXRotr12 = MersenneTwisterEngineSBOXRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                        0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 12>;
using MT19937SBOXRotr13 = MersenneTwisterEngineSBOXRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                        0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 13>;
using MT19937SBOXRotr14 = MersenneTwisterEngineSBOXRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                        0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 14>;
using MT19937SBOXRotr15 = MersenneTwisterEngineSBOXRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                        0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 15>;
using MT19937SBOXRotr16 = MersenneTwisterEngineSBOXRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                        0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 16>;
using MT19937SBOXRotr17 = MersenneTwisterEngineSBOXRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                        0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 17>;
using MT19937SBOXRotr18 = MersenneTwisterEngineSBOXRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                        0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 18>;
using MT19937SBOXRotr19 = MersenneTwisterEngineSBOXRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                        0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 19>;
using MT19937SBOXRotr20 = MersenneTwisterEngineSBOXRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                        0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 20>;
using MT19937SBOXRotr21 = MersenneTwisterEngineSBOXRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                        0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 21>;
using MT19937SBOXRotr22 = MersenneTwisterEngineSBOXRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                        0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 22>;
using MT19937SBOXRotr23 = MersenneTwisterEngineSBOXRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                        0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 23>;
using MT19937SBOXRotr24 = MersenneTwisterEngineSBOXRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                        0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 24>;
using MT19937SBOXRotr25 = MersenneTwisterEngineSBOXRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                        0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 25>;
using MT19937SBOXRotr26 = MersenneTwisterEngineSBOXRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                        0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 26>;
using MT19937SBOXRotr27 = MersenneTwisterEngineSBOXRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                        0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 27>;
using MT19937SBOXRotr28 = MersenneTwisterEngineSBOXRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                        0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 28>;
using MT19937SBOXRotr29 = MersenneTwisterEngineSBOXRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                        0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 29>;
using MT19937SBOXRotr30 = MersenneTwisterEngineSBOXRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                        0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 30>;
using MT19937SBOXRotr31 = MersenneTwisterEngineSBOXRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                        0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 31>;
