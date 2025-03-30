#pragma once

#include "generator.hpp"

#include <array>
#include <cstdint>
#include <immintrin.h>

#ifdef __AVX2__
class MersenneTwister32AVX2 : public Generator<uint32_t> {
  private:
    static constexpr size_t W = 32UL;
    static constexpr size_t N = 624UL;
    static constexpr size_t M = 397UL;
    static constexpr size_t R = 31UL;
    static constexpr size_t A = 0x9908b0dfUL;
    static constexpr size_t U = 11UL;
    static constexpr size_t D = 0xffffffffUL;
    static constexpr size_t S = 7UL;
    static constexpr size_t B = 0x9d2c5680UL;
    static constexpr size_t T = 15UL;
    static constexpr size_t C = 0xefc60000UL;
    static constexpr size_t L = 18UL;
    static constexpr size_t F = 1812433253UL;
    static constexpr uint32_t default_seed = 5489u;
    static constexpr uint32_t UPPER_MASK = (~uint32_t()) << R;
    static constexpr uint32_t LOWER_MASK = ~UPPER_MASK;

    alignas(32) std::array<uint32_t, N> mt;
    size_t index_state;

    void twist();

    // Векторизованное преобразование tempering
    __m256i tempering_simd(__m256i value) const;

    // Скалярное преобразование tempering
    uint32_t tempering_scalar(uint32_t value) const;

  public:
    MersenneTwister32AVX2(const uint32_t &seed = default_seed);

    virtual uint32_t operator()() override;
    virtual uint32_t min() const override;
    virtual uint32_t max() const override;

    void generate_bulk(uint32_t *output, size_t len);
};

using MT32AVX2 = MersenneTwister32AVX2;
#endif

#ifdef __AVX512F__
class MersenneTwister32AVX512 : public Generator<uint32_t> {
  private:
    static constexpr size_t W = 32UL;
    static constexpr size_t N = 624UL;
    static constexpr size_t M = 397UL;
    static constexpr size_t R = 31UL;
    static constexpr size_t A = 0x9908b0dfUL;
    static constexpr size_t U = 11UL;
    static constexpr size_t D = 0xffffffffUL;
    static constexpr size_t S = 7UL;
    static constexpr size_t B = 0x9d2c5680UL;
    static constexpr size_t T = 15UL;
    static constexpr size_t C = 0xefc60000UL;
    static constexpr size_t L = 18UL;
    static constexpr size_t F = 1812433253UL;
    static constexpr uint32_t default_seed = 5489u;
    static constexpr uint32_t UPPER_MASK = (~uint32_t()) << R;
    static constexpr uint32_t LOWER_MASK = ~UPPER_MASK;

    alignas(64) std::array<uint32_t, N> mt;
    size_t index_state;

    void twist();

    // Векторизованное преобразование tempering
    __m512i tempering_simd(__m512i value) const;

    // Скалярное преобразование tempering
    uint32_t tempering_scalar(uint32_t value) const;

  public:
    MersenneTwister32AVX512(const uint32_t &seed = default_seed);

    virtual uint32_t operator()() override;
    virtual uint32_t min() const override;
    virtual uint32_t max() const override;

    void generate_bulk(uint32_t *output, size_t len);
};

using MT32AVX512 = MersenneTwister32AVX512;
#endif
