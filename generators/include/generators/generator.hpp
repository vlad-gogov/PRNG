#pragma once

#include <cstdint>

template <class Type>
class Generator {

  public:
    using result_type = Type;

    virtual Type operator()() = 0;
    virtual Type min() const = 0;
    virtual Type max() const = 0;
    virtual void seed(const Type seed) = 0;
    virtual void discard(const std::uint64_t z) = 0;
};
