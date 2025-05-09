#pragma once

#include "generator.hpp"

#include <cstdint>

template <typename Type>
class LinearGenerator : public Generator<Type> {

    Type _seed;

  public:
    virtual void seed(const Type seed) = 0;
    virtual void discard(const std::uint64_t z) = 0;
};
