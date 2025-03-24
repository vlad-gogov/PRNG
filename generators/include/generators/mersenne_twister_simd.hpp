#pragma once

#include "generator.hpp"

#include <array>
#include <cstdint>
#include <immintrin.h>

constexpr size_t N = 624;
constexpr size_t M = 397;
constexpr uint32_t MATRIX_A = 0x9908b0df;
constexpr uint32_t UPPER_MASK = 0x80000000;
constexpr uint32_t LOWER_MASK = 0x7fffffff;

class MersenneTwister32AVX2 : public Generator<uint32_t> {
  private:
    static constexpr size_t N = 624;
    static constexpr size_t M = 397;
    static constexpr uint32_t MATRIX_A = 0x9908b0df;
    static constexpr uint32_t UPPER_MASK = 0x80000000;
    static constexpr uint32_t LOWER_MASK = 0x7fffffff;

    alignas(32) std::array<uint32_t, N> mt;
    size_t index;

    void twist();

    // Векторизованное преобразование tempering
    __m256i temper(__m256i y) const;

    // Скалярное преобразование tempering
    uint32_t temper_scalar(uint32_t y) const;

  public:
    MersenneTwister32AVX2(const uint32_t &seed = 0);

    virtual uint32_t operator()() override;
    virtual uint32_t min() const override;
    virtual uint32_t max() const override;

    void generate_bulk(uint32_t *output, size_t len);
};
