#pragma once

#include "generator.hpp"

#include <cstdint>

/*&
 * @tparam W  Word size, the number of bits in each element of
 *              the state vector.
 * @tparam N The degree of recursion.
 * @tparam M  The period parameter.
 * @tparam R  The separation point bit index.
 * @tparam A  The last row of the twist matrix.
 * @tparam U  The first right-shift tempering matrix parameter.
 * @tparam D  The first right-shift tempering matrix mask.
 * @tparam S  The first left-shift tempering matrix parameter.
 * @tparam B  The first left-shift tempering matrix mask.
 * @tparam T  The second left-shift tempering matrix parameter.
 * @tparam C  The second left-shift tempering matrix mask.
 * @tparam L  The second right-shift tempering matrix parameter.
 * @tparam F  Initialization multiplier.
 */

template <typename UIntType, size_t W, size_t N, size_t M, size_t R, UIntType A, size_t U, UIntType D, size_t S,
          UIntType B, size_t T, UIntType C, size_t L, UIntType F>
class MersenneTwisterEngine : Generator<UIntType> {

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
        for (int i = 0; i < N; ++i) {
            unsigned int x = (mt[i] & UPPER_MASK) + (mt[(i + 1) % N] & LOWER_MASK);
            unsigned int xA = x >> 1;
            if (x % 2 != 0) {
                xA ^= A;
            }
            mt[i] = mt[(i + M) % N] ^ xA;
        }
        index_state = 0;
    }

    static constexpr UIntType default_seed = 5489u;

    UIntType mt[N]; // array state
    size_t index_state;

  public:
    MersenneTwisterEngine(const UIntType seed = default_seed) {
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

    UIntType operator()() noexcept override {
        if (index_state >= N) {
            twist();
        }

        UIntType y = mt[index_state];
        y ^= (y >> U) & D;
        y ^= (y << S) & B;
        y ^= (y << T) & C;
        y ^= (y >> L);
        index_state++;

        return y;
    }

    void discard(uint64_t z) {
        for (; 0 < z; --z) {
            this->operator()();
        }
    }
};

using MT19937 = MersenneTwisterEngine<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15,
                                      0xefc60000UL, 18, 1812433253UL>;

// template <typename UIntType, size_t W, size_t N, size_t M, size_t R, UIntType A, size_t U, UIntType D, size_t S,
//          UIntType B, size_t T, UIntType C, size_t L, UIntType F>
using MT19937_64 =
    MersenneTwisterEngine<uint64_t, 64, 312, 156, 31, 0xb5026f5aa96619e9ULL, 29, 0x5555555555555555ULL, 17,
                          0x71d67fffeda60000ULL, 37, 0xfff7eee000000000ULL, 43, 6364136223846793005ULL>;

/*&
 * @tparam W  Word size, the number of bits in each element of
 *              the state vector.
 * @tparam N The degree of recursion.
 * @tparam M0  The period parameter.
 * @tparam M1  The period parameter.
 * @tparam M2  The period parameter.
 * @tparam R  The separation point bit index.
 * @tparam A  The last row of the twist matrix.
 * @tparam U  The first right-shift tempering matrix parameter.
 * @tparam D  The first right-shift tempering matrix mask.
 * @tparam S  The first left-shift tempering matrix parameter.
 * @tparam B  The first left-shift tempering matrix mask.
 * @tparam T  The second left-shift tempering matrix parameter.
 * @tparam C  The second left-shift tempering matrix mask.
 * @tparam L  The second right-shift tempering matrix parameter.
 * @tparam F  Initialization multiplier.
 */

template <typename UIntType, size_t W, size_t N, size_t M0, size_t M1, size_t M2, size_t R, UIntType A, size_t U,
          size_t S, UIntType B, size_t T, UIntType C, size_t L, UIntType F>
class MersenneTwisterEngine64 : Generator<UIntType> {

  protected:
    static_assert(std::is_unsigned<UIntType>::value, "result_type must be an unsigned integral type");
    static_assert(1u <= M0 && M0 <= N, "template argument substituting M0 out of bounds");
    static_assert(1u <= M1 && M1 <= N, "template argument substituting M1 out of bounds");
    static_assert(1u <= M2 && M2 <= N, "template argument substituting M2 out of bounds");
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
    static_assert(F <= (UIntType(-1)), "template argument substituting F out of bound");

    void twist() {
        const UIntType UPPER_MASK = (~UIntType()) << R;
        const UIntType LOWER_MASK = ~UPPER_MASK;
        for (int i = 0; i < N; ++i) {
            unsigned int x = (mt[i] & UPPER_MASK) + (mt[(i + 1) % N] & LOWER_MASK);
            unsigned int xA = x >> 1;
            if (x % 2 != 0) {
                xA ^= A;
            }
            mt[i] = mt[(i + M0) % N] ^ mt[(i + M1) % N] ^ mt[(i + M2) % N] ^ xA;
        }
        index_state = 0;
    }

    static constexpr UIntType default_seed = 5489u;

    UIntType mt[N]; // array state
    size_t index_state;

  public:
    MersenneTwisterEngine64(const UIntType seed = default_seed) {
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

    UIntType operator()() noexcept override {
        if (index_state >= N) {
            twist();
        }

        UIntType y = mt[index_state];
        y ^= (y >> U);
        y ^= (y << S) & B;
        y ^= (y << T) & C;
        y ^= (y >> L);
        index_state++;

        return y;
    }

    void discard(uint64_t z) {
        for (; 0 < z; --z) {
            this->operator()();
        }
    }
};

// template <typename UIntType, size_t W, size_t N, size_t M0, size_t M1, size_t M2, size_t R, UIntType A, size_t U,
//           size_t S, UIntType B, size_t T, UIntType C, size_t L, UIntType F>

using MT19937_64_1 =
    MersenneTwisterEngine64<uint64_t, 64, 312, 63, 151, 224, 31, 0xB3815B624FC82E2FULL, 26, 17, 0x656BEDFFD9A40000ULL,
                            33, 0xFFFAAFFE00000000ULL, 39, 6364136223846793005ULL>;

using MT19937_64_2 =
    MersenneTwisterEngine64<uint64_t, 64, 312, 55, 122, 268, 31, 0x8EBD4AD46CB39A1EULL, 26, 17, 0xA51DBEFFDA6C0000ULL,
                            33, 0xFDFECE7E00000000ULL, 39, 6364136223846793005ULL>;

using MT19937_64_3 =
    MersenneTwisterEngine64<uint64_t, 64, 312, 87, 148, 241, 31, 0xCACB98F78EBCD4EDULL, 26, 17, 0x599CFCBFCA660000ULL,
                            33, 0xFFEE9BF600000000ULL, 39, 6364136223846793005ULL>;