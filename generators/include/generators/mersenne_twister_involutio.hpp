#include "generators/mersenne_twister_sbox.hpp"

template <typename UIntType, size_t W, size_t N, size_t M, size_t R, UIntType A, size_t U, UIntType D, size_t S,
          UIntType B, size_t T, UIntType C, size_t L, UIntType F, UIntType shift>
class MersenneTwisterEngineInvolution : public Generator<UIntType> {
    MersenneTwisterEngineSBox<UIntType, W, N, M, R, A, U, D, S, B, T, C, L, F> mt_gen;
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
        mt_gen = MersenneTwisterEngineSBox<uint32_t, W, N, M, R, A, U, D, S, B, T, C, L, F>(seed);
    }

    void discard(const std::uint64_t z) override {
        for (size_t i = 0; i < z; ++i) {
            this->operator()();
        }
    }
};

using MT19937Involution3 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                           7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 3>;

using MT19937Involution14 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                            7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 14>;

using MT19937Involution24 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                            7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 24>;

using MT19937Involution26 = MersenneTwisterEngineInvolution<uint32_t, 32, 624, 397, 31, 0x9908b0dfUL, 11, 0xffffffffUL,
                                                            7, 0x9d2c5680UL, 15, 0xefc60000UL, 18, 1812433253UL, 26>;
