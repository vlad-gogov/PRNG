#include "generators/mersenne_twister.hpp"
#include "generators/mersenne_twister_sbox.hpp"

#include <bit>

template <typename UIntType, size_t W, size_t N, size_t M, size_t R, UIntType A, size_t U, UIntType D, size_t S,
          UIntType B, size_t T, UIntType C, size_t L, UIntType F, UIntType shift>
class MersenneTwisterEngineRotr : public Generator<UIntType> {
    MersenneTwisterEngine<UIntType, W, N, M, R, A, U, D, S, B, T, C, L, F> mt_gen;
    static constexpr UIntType default_seed = 5489u;

  public:
    MersenneTwisterEngineRotr(const UIntType seed = default_seed) : mt_gen(seed) {
    }

    UIntType operator()() noexcept override {
        constexpr size_t bytes = sizeof(UIntType) * 8;
        UIntType value = mt_gen();
        UIntType result = std::rotr(value, shift);
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

using MT19937Rotr1 = MersenneTwisterEngineRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                               0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 1>;
using MT19937Rotr2 = MersenneTwisterEngineRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                               0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 2>;

using MT19937Rotr3 = MersenneTwisterEngineRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                               0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 3>;

using MT19937Rotr4 = MersenneTwisterEngineRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                               0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 4>;

using MT19937Rotr5 = MersenneTwisterEngineRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                               0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 5>;
using MT19937Rotr6 = MersenneTwisterEngineRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                               0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 6>;
using MT19937Rotr7 = MersenneTwisterEngineRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                               0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 7>;
using MT19937Rotr8 = MersenneTwisterEngineRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                               0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 8>;
using MT19937Rotr9 = MersenneTwisterEngineRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                               0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 9>;
using MT19937Rotr10 = MersenneTwisterEngineRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 10>;
using MT19937Rotr11 = MersenneTwisterEngineRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 11>;
using MT19937Rotr12 = MersenneTwisterEngineRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 12>;
using MT19937Rotr13 = MersenneTwisterEngineRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 13>;

using MT19937Rotr14 = MersenneTwisterEngineRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 14>;

using MT19937Rotr15 = MersenneTwisterEngineRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 15>;
using MT19937Rotr16 = MersenneTwisterEngineRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 16>;
using MT19937Rotr17 = MersenneTwisterEngineRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 17>;
using MT19937Rotr18 = MersenneTwisterEngineRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 18>;
using MT19937Rotr19 = MersenneTwisterEngineRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 19>;
using MT19937Rotr20 = MersenneTwisterEngineRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 20>;
using MT19937Rotr21 = MersenneTwisterEngineRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 21>;
using MT19937Rotr22 = MersenneTwisterEngineRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 22>;
using MT19937Rotr23 = MersenneTwisterEngineRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 23>;
using MT19937Rotr24 = MersenneTwisterEngineRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 24>;

using MT19937Rotr25 = MersenneTwisterEngineRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 25>;
using MT19937Rotr26 = MersenneTwisterEngineRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 26>;

using MT19937Rotr27 = MersenneTwisterEngineRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 27>;
using MT19937Rotr28 = MersenneTwisterEngineRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 28>;
using MT19937Rotr29 = MersenneTwisterEngineRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 29>;
using MT19937Rotr30 = MersenneTwisterEngineRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 30>;
using MT19937Rotr31 = MersenneTwisterEngineRotr<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL, 7,
                                                0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 31>;
