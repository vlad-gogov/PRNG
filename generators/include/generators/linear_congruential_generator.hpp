#pragma once

#include "base_error.hpp"
#include "linear_generator.hpp"

#include <type_traits>

template <class UIntType, UIntType a, UIntType c, UIntType m>
class LinearCongruentialGenerator : LinearGenerator<UIntType> {

    UIntType _seed;

  public:
    explicit LinearCongruentialGenerator(UIntType seed = 1U) {
        if (std::is_floating_point_v<UIntType> || std::is_signed_v<UIntType>) {
            throw BaseError("LCG can work with unsigned integers");
        }
        if (a >= m) {
            throw BaseError("Incorrect multiplier");
        }
        if (c >= m) {
            throw BaseError("Incorrect increment");
        }
        if (seed >= m) {
            throw BaseError("Incorrect seed");
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
        for (; 0 < z; --z) {
            std::cout << this->operator()() << std::endl;
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
