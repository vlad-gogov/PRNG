#pragma once

#include <iostream>

template <class UIntType, UIntType a, UIntType c, UIntType m>
class LinearCongruentialGenerator {

    UIntType _seed;

  public:
    LinearCongruentialGenerator(UIntType seed = 1U) {
        _seed = seed;
    }
    
    UIntType operator()() noexcept {
        return (_seed = (a * _seed + c) % m);
    }

    void discard(std::uint_fast64_t z) {
        for (std::uint_fast64_t i = 0; i < z; ++i) {
            this->operator();
        }
    }
};