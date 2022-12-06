#pragma once

#include "base_error.hpp"
#include "linear_generator.hpp"

#include <iostream>
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
        if (a == 0) {
            _seed = static_cast<UIntType>(c);
        } else if (m == 0) {
            _seed = static_cast<UIntType>(static_cast<UIntType>(a * _seed) + c);
        } else if (c <= UINT_MAX && static_cast<UIntType>(m - 1) <= (UINT_MAX - c) / a) {
            const auto mul = static_cast<unsigned int>(_seed) * static_cast<unsigned int>(a) + static_cast<unsigned int>(c);
            _seed = static_cast<UIntType>(mul % m);
        } else if (c <= ULLONG_MAX && static_cast<UIntType>(m - 1) <= (ULLONG_MAX - c) / a) {
            const auto mul = static_cast<unsigned long long>(_seed) * a + c;
            _seed = static_cast<UIntType>(mul % m);
        } else {
            _seed = static_cast<UIntType>(static_cast<UIntType>(static_cast<UIntType>(a) * _seed) + c);
            _seed = static_cast<UIntType>(_seed % m);
        }
        return _seed;
    }

    void discard(std::uint_fast64_t z) {
        for (; 0 < z; --z) {
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
