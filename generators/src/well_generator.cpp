#include "generators/well_generator.hpp"

WELL512a::WELL512a(uint32_t seed) {
    this->seed(seed);
}

void WELL512a::seed(uint32_t s) {
    // Initialize state array using LCG
    state[0] = s;
    for (int i = 1; i < STATE_SIZE; i++) {
        state[i] = (1812433253UL * (state[i-1] ^ (state[i-1] >> 30)) + i);
    }
    index = 0;
}

void WELL512a::discard(std::uint64_t z) {
    for (; z > 0; --z) {
        operator()();
    }
}

uint32_t WELL512a::operator()() noexcept {
    uint32_t z0, z1, z2;
    
    z0 = state[(index + 15) & 0x0f];
    z1 = MAT0POS(16, state[index]) ^ MAT0NEG(15, state[(index + M1) & 0x0f]);
    z2 = MAT0NEG(2, state[(index + M2) & 0x0f]) ^ MAT0NEG(18, state[(index + M3) & 0x0f]);
    
    state[index] = z1 ^ z2;
    state[(index + 15) & 0x0f] = MAT0NEG(7, z0) ^ MAT0NEG(16, z1) ^ MAT4NEG(12, 0xfdff37ffU, z2);
    
    index = (index + 15) & 0x0f;
    
    return state[index];
}
