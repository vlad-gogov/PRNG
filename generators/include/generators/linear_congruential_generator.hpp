#pragma once

#include "linear_generator.hpp"

template <class UIntType, UIntType a, UIntType c, UIntType m>
class LinearCongruentialGenerator : LinearGenerator<UIntType> {

    UIntType _seed;

  public:
    explicit LinearCongruentialGenerator(UIntType seed = 1U) {
        if (m <= 0) {
            throw "Incorrect modulus";
        }
        if (a <= 0 || a >= m) {
            throw "Incorrect multiplier";
        }
        if (c < 0 || c >= m) {
            throw "Incorrect increment";
        }
        if (seed < 0 || seed >= m) {
            throw "Incorrect seed";
        }
        _seed = seed;
    }

    void seed(UIntType seed) {
        _seed = seed;
    }
    
    UIntType operator()() noexcept {
        return (_seed = (a * _seed + c) % m);
    }

    void discard(std::uint_fast64_t z) {
        for (std::uint_fast64_t i = 0; i < z; ++i) {
            this->operator()();
        }
    }

    UIntType min() {
        if (c == 0) {
            return static_cast<UIntType>(1);
        }
        return static_cast<UIntType>(0);
    }

    UIntType max() {
        return m - static_cast<UIntType>(1);
    }
};
