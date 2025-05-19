#pragma once

#include <array>
#include <bit>
#include <vector>

#include "generators/generator.hpp"
#include "generators/mersenne_twister_sbox.hpp"

template <typename UIntType, size_t W, size_t N, size_t M, size_t R, UIntType A, size_t U, UIntType D, size_t S,
          UIntType B, size_t T, UIntType C, size_t L, UIntType F, size_t shift>
class MersenneTwisterEngineSBOXRotr : public Generator<UIntType> {

    static_assert(std::is_unsigned<UIntType>::value, "result_type must be an unsigned integral type");
    static_assert(1u <= M && M <= N, "template argument substituting M out of bounds");
    static_assert(W <= std::numeric_limits<UIntType>::digits, "template argument substituting __w out of bound");
    static_assert(R <= W, "template argument substituting "
                          "R out of bound");
    static_assert(U <= W, "template argument substituting "
                          "U out of bound");
    static_assert(S <= W, "template argument substituting "
                          "S out of bound");
    static_assert(T <= W, "template argument substituting "
                          "T out of bound");
    static_assert(L <= W, "template argument substituting "
                          "L out of bound");
    static_assert(A <= (UIntType(-1)), "template argument substituting A out of bound");
    static_assert(B <= (UIntType(-1)), "template argument substituting B out of bound");
    static_assert(C <= (UIntType(-1)), "template argument substituting C out of bound");
    static_assert(D <= (UIntType(-1)), "template argument substituting D out of bound");
    static_assert(F <= (UIntType(-1)), "template argument substituting F out of bound");

    void twist() {
        const UIntType UPPER_MASK = (~UIntType()) << R;
        const UIntType LOWER_MASK = ~UPPER_MASK;
        for (size_t i = 0; i < N; ++i) {
            size_t x = (mt[i] & UPPER_MASK) | (mt[(i + 1) % N] & LOWER_MASK);
            size_t xA = x >> 1;
            if (x % 2 != 0) {
                xA ^= A;
            }
            mt[i] = mt[(i + M) % N] ^ xA;
        }
        index_state = 0;
    }

    static constexpr UIntType default_seed = 5489u;

    UIntType mt[N];
    size_t index_state;

  public:
    MersenneTwisterEngineSBOXRotr(const UIntType seed = default_seed) {
        mt[0] = seed;
        for (size_t i = 1; i < N; i++) {
            mt[i] = (F * (mt[i - 1] ^ (mt[i - 1] >> (W - 2))) + i);
        }
        index_state = N;
    }

    UIntType min() const override {
        return static_cast<UIntType>(0);
    }

    UIntType max() const override {
        return static_cast<UIntType>(1) << W - 1;
    }

    UIntType tempering(UIntType y) noexcept {
        constexpr size_t bytes = sizeof(UIntType);
        y ^= (y >> U) & D;
        y ^= (y << S) & B;
        y ^= (y << T) & C;
        y ^= (y >> L);
        UIntType result = 0u;
        for (size_t i = 0; i < bytes; ++i) {
            result |= (AES_SBOX[(y >> (i * 8)) & 0xFF]) << ((bytes - i - 1) * 8);
        }
        result = std::rotr(result, shift);
        return result;
    }

    UIntType operator()() noexcept override {
        constexpr size_t bytes = sizeof(UIntType);
        if (index_state >= N) {
            twist();
        }
        UIntType y = mt[index_state++];
        y ^= (y >> U) & D;
        y ^= (y << S) & B;
        y ^= (y << T) & C;
        y ^= (y >> L);
        UIntType result = 0u;
        uint8_t *y_ptr = reinterpret_cast<uint8_t *>(&y);
        uint8_t *res_ptr = reinterpret_cast<uint8_t *>(&result);
        for (size_t i = 0; i < bytes; ++i) {
            res_ptr[bytes - i - 1] = AES_SBOX[y_ptr[i]];
        }
        result = std::rotr(result, shift);
        return result;
    }

    void seed(const UIntType seed) override {
        *this = MersenneTwisterEngineSBOXRotr(seed);
    }

    void discard(uint64_t z) override {
        for (; 0 < z; --z) {
            this->operator()();
        }
    }

    UIntType random_raw() noexcept {
        if (index_state >= N) {
            twist();
        }
        return mt[index_state++];
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
