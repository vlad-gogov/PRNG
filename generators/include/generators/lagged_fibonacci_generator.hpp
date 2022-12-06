#pragma once

#include <deque>
#include <limits>
#include "linear_generator.hpp"

template <class UIntType, UIntType j, UIntType k, UIntType m>
class LaggedFibonacciGenerator : LinearGenerator<UIntType> {

    std::deque<UIntType> _seed;
    char _carry;

  public:
    explicit LaggedFibonacciGenerator(UIntType seed_word) {
        if (m <= 0 | m > numeric_limits<UIntType>::digits) {
            throw "Incorrect modulus";
        }
        if (k <= 0) {
            throw "Incorrect long lag value";
        }
        if (j <= 0 || j >= k) {
            throw "Incorrect short lag value";
        }
        if (seed < 0) {
            throw "Incorrect seed value";
        }
        std::linear_congruential_engine<std::uint_fast32_t, seed_word, 5U, (2 << 31) - 1> seed_generator(seed_word);
        for (std::uint_fast64_t i = 0; i < k; ++i) {
            _seed.append(seed_generator());
        }
        _carry = 0;
    }

    void seed(std::deque<UIntType> seed) {
        _seed = seed;
    }
    
    UIntType operator()() noexcept {
        UIntType new_element = _seed[k - 1] - _seed[j - 1] - _carry;
        new_element % 2 ? (_carry = 1) : (_carry = 0);
        UIntType new_element = new_element % m;
        _seed.erase(_seed.end());
        _seed.insert(_seed.begin(), new_element);
        return new_element;
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
        return (2 << m) - static_cast<UIntType>(1);
    }
};
