#pragma once

#include "generator.hpp"

#include <cstdint>

template <typename Type>
class LinearGenerator : Generator<Type> {

    Type _seed;

  public:
    virtual void seed(Type seed) = 0;
    virtual void discard(std::uint64_t z) = 0;
    virtual Type operator()() noexcept = 0;
    virtual Type min() = 0;
    virtual Type max() = 0;
};
