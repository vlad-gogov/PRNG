#include "generators/mersenne_twister.hpp"
#include "generators/mersenne_twister_sbox.hpp"

template <typename UIntType, size_t W, size_t N, size_t M, size_t R, UIntType A, size_t U, UIntType D, size_t S,
          UIntType B, size_t T, UIntType C, size_t L, UIntType F, UIntType shift>
class MersenneTwisterEngineInvolution : public Generator<UIntType> {
    MersenneTwisterEngine<UIntType, W, N, M, R, A, U, D, S, B, T, C, L, F> mt_gen;
    static constexpr UIntType default_seed = 5489u;

  public:
    MersenneTwisterEngineInvolution(const UIntType seed = default_seed) : mt_gen(default_seed) {
    }

    UIntType operator()() noexcept override {
        constexpr size_t bytes = sizeof(UIntType) * 8;
        UIntType value = mt_gen();
        UIntType result = (value << shift) | (value >> (bytes - shift));
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

using MT19937Involution1 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                           7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 1>;
using MT19937Involution2 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                           7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 2>;

using MT19937Involution3 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                           7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 3>;

using MT19937Involution4 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                           7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 4>;

using MT19937Involution5 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                           7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 5>;
using MT19937Involution6 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                           7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 6>;
using MT19937Involution7 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                           7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 7>;
using MT19937Involution8 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                           7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 8>;
using MT19937Involution9 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                           7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 9>;
using MT19937Involution10 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                            7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 10>;
using MT19937Involution11 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                            7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 11>;
using MT19937Involution12 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                            7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 12>;
using MT19937Involution13 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                            7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 13>;

using MT19937Involution14 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                            7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 14>;

using MT19937Involution15 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                            7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 15>;
using MT19937Involution16 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                            7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 16>;
using MT19937Involution17 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                            7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 17>;
using MT19937Involution18 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                            7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 18>;
using MT19937Involution19 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                            7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 19>;
using MT19937Involution20 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                            7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 20>;
using MT19937Involution21 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                            7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 21>;
using MT19937Involution22 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                            7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 22>;
using MT19937Involution23 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                            7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 23>;
using MT19937Involution24 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                            7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 24>;

using MT19937Involution25 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                            7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 25>;
using MT19937Involution26 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                            7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 26>;

using MT19937Involution27 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                            7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 27>;
using MT19937Involution28 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                            7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 28>;
using MT19937Involution29 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                            7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 29>;
using MT19937Involution30 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                            7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 30>;
using MT19937Involution31 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                            7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 31>;
