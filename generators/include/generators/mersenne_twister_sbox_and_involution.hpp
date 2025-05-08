#pragma once

#include <array>
#include <vector>

#include "generators/generator.hpp"
#include "generators/mersenne_twister_sbox.hpp"

template <typename UIntType, size_t W, size_t N, size_t M, size_t R, UIntType A, size_t U, UIntType D, size_t S,
          UIntType B, size_t T, UIntType C, size_t L, UIntType F, UIntType shift>
class MersenneTwisterEngineSBOXInvolution : public Generator<uint32_t> {
    MersenneTwisterEngine<UIntType, W, N, M, R, A, U, D, S, B, T, C, L, F> mt_gen;
    static constexpr UIntType default_seed = 5489u;

  public:
    MersenneTwisterEngineSBOXInvolution(const UIntType seed = default_seed) : mt_gen(seed) {
    }

    UIntType operator()() noexcept override {
        constexpr size_t bytes = sizeof(UIntType);
        UIntType raw_val = mt_gen();
        UIntType result = 0u;
        for (size_t i = 0; i < bytes; ++i) {
            result |= (AES_SBOX[(raw_val >> (i * 8)) & 0xFF]) << ((bytes - i - 1) * 8);
        }
        result = (result << shift) | (result >> (bytes - shift));
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

using MT19937SBOXInvolution1 =
    MersenneTwisterEngineSBOXInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                        0xefc60000UL, 18, 1812433253UL, 1>;
using MT19937SBOXInvolution2 =
    MersenneTwisterEngineSBOXInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                        0xefc60000UL, 18, 1812433253UL, 2>;
using MT19937SBOXInvolution3 =
    MersenneTwisterEngineSBOXInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                        0xefc60000UL, 18, 1812433253UL, 3>;
using MT19937SBOXInvolution4 =
    MersenneTwisterEngineSBOXInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                        0xefc60000UL, 18, 1812433253UL, 4>;
using MT19937SBOXInvolution5 =
    MersenneTwisterEngineSBOXInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                        0xefc60000UL, 18, 1812433253UL, 5>;
using MT19937SBOXInvolution6 =
    MersenneTwisterEngineSBOXInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                        0xefc60000UL, 18, 1812433253UL, 6>;
using MT19937SBOXInvolution7 =
    MersenneTwisterEngineSBOXInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                        0xefc60000UL, 18, 1812433253UL, 7>;
using MT19937SBOXInvolution8 =
    MersenneTwisterEngineSBOXInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                        0xefc60000UL, 18, 1812433253UL, 8>;
using MT19937SBOXInvolution9 =
    MersenneTwisterEngineSBOXInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                        0xefc60000UL, 18, 1812433253UL, 9>;
using MT19937SBOXInvolution10 =
    MersenneTwisterEngineSBOXInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                        0xefc60000UL, 18, 1812433253UL, 10>;
using MT19937SBOXInvolution11 =
    MersenneTwisterEngineSBOXInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                        0xefc60000UL, 18, 1812433253UL, 11>;
using MT19937SBOXInvolution12 =
    MersenneTwisterEngineSBOXInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                        0xefc60000UL, 18, 1812433253UL, 12>;
using MT19937SBOXInvolution13 =
    MersenneTwisterEngineSBOXInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                        0xefc60000UL, 18, 1812433253UL, 13>;
using MT19937SBOXInvolution14 =
    MersenneTwisterEngineSBOXInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                        0xefc60000UL, 18, 1812433253UL, 14>;
using MT19937SBOXInvolution15 =
    MersenneTwisterEngineSBOXInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                        0xefc60000UL, 18, 1812433253UL, 15>;
using MT19937SBOXInvolution16 =
    MersenneTwisterEngineSBOXInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                        0xefc60000UL, 18, 1812433253UL, 16>;
using MT19937SBOXInvolution17 =
    MersenneTwisterEngineSBOXInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                        0xefc60000UL, 18, 1812433253UL, 17>;
using MT19937SBOXInvolution18 =
    MersenneTwisterEngineSBOXInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                        0xefc60000UL, 18, 1812433253UL, 18>;
using MT19937SBOXInvolution19 =
    MersenneTwisterEngineSBOXInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                        0xefc60000UL, 18, 1812433253UL, 19>;
using MT19937SBOXInvolution20 =
    MersenneTwisterEngineSBOXInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                        0xefc60000UL, 18, 1812433253UL, 20>;
using MT19937SBOXInvolution21 =
    MersenneTwisterEngineSBOXInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                        0xefc60000UL, 18, 1812433253UL, 21>;
using MT19937SBOXInvolution22 =
    MersenneTwisterEngineSBOXInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                        0xefc60000UL, 18, 1812433253UL, 22>;
using MT19937SBOXInvolution23 =
    MersenneTwisterEngineSBOXInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                        0xefc60000UL, 18, 1812433253UL, 23>;
using MT19937SBOXInvolution24 =
    MersenneTwisterEngineSBOXInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                        0xefc60000UL, 18, 1812433253UL, 24>;
using MT19937SBOXInvolution25 =
    MersenneTwisterEngineSBOXInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                        0xefc60000UL, 18, 1812433253UL, 25>;
using MT19937SBOXInvolution26 =
    MersenneTwisterEngineSBOXInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                        0xefc60000UL, 18, 1812433253UL, 26>;
using MT19937SBOXInvolution27 =
    MersenneTwisterEngineSBOXInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                        0xefc60000UL, 18, 1812433253UL, 27>;
using MT19937SBOXInvolution28 =
    MersenneTwisterEngineSBOXInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                        0xefc60000UL, 18, 1812433253UL, 28>;
using MT19937SBOXInvolution29 =
    MersenneTwisterEngineSBOXInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                        0xefc60000UL, 18, 1812433253UL, 29>;
using MT19937SBOXInvolution30 =
    MersenneTwisterEngineSBOXInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                        0xefc60000UL, 18, 1812433253UL, 30>;
using MT19937SBOXInvolution31 =
    MersenneTwisterEngineSBOXInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                        0xefc60000UL, 18, 1812433253UL, 31>;
