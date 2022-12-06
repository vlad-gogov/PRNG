#pragma once

#include <deque>
#include <limits>
#include <random>
#include "linear_generator.hpp"

template <class UIntType, UIntType m, UIntType j, UIntType k>
class LaggedFibonacciGenerator : LinearGenerator<UIntType> {

    std::deque<UIntType> _seed_deque;
    UIntType _seed;
    char _carry;

  public:
    explicit LaggedFibonacciGenerator(UIntType seed_word = 16807U) {
        if (m <= 0 | m > std::numeric_limits<UIntType>::digits) {
            throw "Incorrect modulus";
        }
        if (k <= 0) {
            throw "Incorrect long lag value";
        }
        if (j <= 0 || j >= k) {
            throw "Incorrect short lag value";
        }
        if (seed_word < 0) {
            throw "Incorrect seed value";
        }
        std::linear_congruential_engine<std::uint_fast32_t, 19780503, 0, 2147483647> seed_generator(seed_word);
        for (std::uint_fast64_t i = 0; i < k; ++i) {
            _seed_deque.push_front(seed_generator());
        }
        _carry = 0;
        _seed = seed_word;
    }

    void seed(UIntType seed) {
        _seed = seed;
    }
    
    UIntType operator()() noexcept {
        UIntType new_element = _seed_deque[k - 1] - _seed_deque[j - 1] - _carry;
        new_element % 2 ? (_carry = 1) : (_carry = 0);
        new_element = new_element % (2 << (m - 2));
        _seed_deque.pop_back();
        _seed_deque.push_front(new_element);
        return new_element;
    }

    void discard(std::uint_fast64_t z) {
        for (; 0 < z; --z) {
            this->operator()();
        }
    }

    UIntType min() {
        return static_cast<UIntType>(0);
    }

    UIntType max() {
        return std::numeric_limits<UIntType>::max();
    }
};
