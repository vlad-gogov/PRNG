#pragma once

#include "generator.hpp"

#include <cstdint>

template <typename Type>
class LinearGenerator : Generator<Type> {

    Type _seed;

  public:
    virtual void seed(Type seed) = 0;
    virtual void discard(std::uint64_t z) = 0;
};
