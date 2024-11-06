#pragma once

#include "linear_generator.hpp"
#include <array>
#include <cstdint>

// WELL512a implementation
// Based on "Better than Mersenne Twister" paper by F. Panneton, P. L'Ecuyer and M. Matsumoto
class WELL512a : public LinearGenerator<uint32_t> {
private:
    static constexpr int STATE_SIZE = 16;  // R = 16 for WELL512
    std::array<uint32_t, STATE_SIZE> state;
    int index;

    // WELL512a tempering parameters
    static constexpr uint32_t M1 = 13;
    static constexpr uint32_t M2 = 9;
    static constexpr uint32_t M3 = 5;

    uint32_t MAT0POS(uint32_t t, uint32_t v) { return v ^ (v >> t); }
    uint32_t MAT0NEG(uint32_t t, uint32_t v) { return v ^ (v << t); }
    uint32_t MAT3NEG(uint32_t t, uint32_t v) { return v << t; }
    uint32_t MAT4NEG(uint32_t t, uint32_t b, uint32_t v) { return v ^ ((v << t) & b); }

public:
    explicit WELL512a(uint32_t seed = 19650218U);
    
    // Implement LinearGenerator interface
    void seed(uint32_t s) override;
    void discard(std::uint64_t z) override;
    uint32_t operator()() noexcept;
    
    static constexpr uint32_t min() { return 0; }
    static constexpr uint32_t max() { return 0xFFFFFFFFU; }
};
