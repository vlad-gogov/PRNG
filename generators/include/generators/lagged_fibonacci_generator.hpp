#pragma once

#include "linear_generator.hpp"
#include <deque>
#include <limits>
#include <random>

template <class UIntType, UIntType m, UIntType j, UIntType k>
class LaggedFibonacciGenerator : LinearGenerator<UIntType> {

    std::deque<UIntType> vals;

  public:
    explicit LaggedFibonacciGenerator(UIntType seed_word = 16807U) {
        if (m <= 0 || m > std::numeric_limits<UIntType>::max()) {
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
        std::linear_congruential_engine<std::uint32_t, 19780503, 0, m> seed_generator(seed_word);
        for (std::uint64_t i = 0; i < k + 1; ++i)
            vals.push_back(seed_generator());
        if (vals[0] % 2 == 0) {
            if (vals[0] == 0)
                vals[0]++;
            vals[0]--;
        }
    }

    UIntType operator()() noexcept {
        UIntType new_element = ((vals[0] % m) + (vals[k - j] % m)) % m;
        vals.push_back(new_element);
        vals.pop_front();
        return new_element;
    }

    void discard(std::uint64_t z) {
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
