#pragma once

#include "base_error.hpp"
#include "linear_generator.hpp"

#include <climits>
#include <iostream>
#include <type_traits>

template <class UIntType, UIntType a, UIntType c, UIntType m>
class LinearCongruentialGenerator : LinearGenerator<UIntType> {
    static_assert(std::is_integral_v<UIntType> && std::is_unsigned_v<UIntType>);

    UIntType _seed;

  public:
    explicit LinearCongruentialGenerator(const UIntType seed = 1U) {
        static_assert(a < m, "Incorrect multiplier");
        static_assert(c < m, "Incorrect increment");
        if (seed >= m) {
            throw BaseError("Incorrect seed");
        }
        _seed = seed;
    }

    void seed(const UIntType seed) override {
        _seed = seed;
    }

    UIntType operator()() noexcept override {
        if (a == 0) {
            _seed = static_cast<UIntType>(c);
        } else if (m == 0) {
            _seed = static_cast<UIntType>(static_cast<UIntType>(a * _seed) + c);
        } else if (c <= UINT_MAX && static_cast<UIntType>(m - 1) <= (UINT_MAX - c) / a) {
            const auto mul =
                static_cast<unsigned int>(_seed) * static_cast<unsigned int>(a) + static_cast<unsigned int>(c);
            _seed = static_cast<UIntType>(mul % m);
        } else if (c <= ULLONG_MAX && static_cast<UIntType>(m - 1) <= (ULLONG_MAX - c) / a) {
            const auto mul = static_cast<unsigned long long>(_seed) * a + c;
            _seed = static_cast<UIntType>(mul % m);
        } else {
            _seed = static_cast<UIntType>(((a * _seed) + c) % m);
        }
        return _seed;
    }

    void discard(std::uint64_t z) override {
        for (; 0 < z; --z) {
            this->operator()();
        }
    }

    UIntType min() const override {
        if (c == 0) {
            return static_cast<UIntType>(1);
        }
        return static_cast<UIntType>(0);
    }

    UIntType max() const override {
        return m - static_cast<UIntType>(1);
    }
};
