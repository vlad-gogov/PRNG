#pragma once

#include "generator.hpp"

#include <array>
#include <random>

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

template <typename UIntType, size_t N, size_t M, size_t R, UIntType A, size_t U, UIntType D, size_t S, UIntType B,
          size_t T, UIntType C, size_t L, UIntType F>
class MersenneTwisterEngine : Generator<UIntType> {

    static_assert(std::is_unsigned<UIntType>::value, "result_type must be an unsigned integral type");
    static_assert(1u <= M && M <= N, "template argument substituting M out of bounds");
    static constexpr size_t W = std::numeric_limits<UIntType>::digits;
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
    static_assert(A <= (size_t(1) << W) - 1, "template argument substituting A out of bound");
    static_assert(B <= (size_t(1) << W) - 1, "template argument substituting B out of bound");
    static_assert(C <= (size_t(1) << W) - 1, "template argument substituting C out of bound");
    static_assert(D <= (size_t(1) << W) - 1, "template argument substituting D out of bound");
    static_assert(F <= (size_t(1) << W) - 1, "template argument substituting F out of bound");

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

  public:
    static constexpr UIntType default_seed = 5489u;

    UIntType mt[N]; // array state
    size_t index_state;

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

    void discard(std::uint64_t z) {
        for (; 0 < z; --z) {
            this->operator()();
        }
    }
};

typedef MersenneTwisterEngine<uint32_t, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15, 0xefc60000UL,
                              18, 1812433253UL>
    MT19937;
