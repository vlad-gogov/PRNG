#pragma once

template <class Type>
class Generator {

  public:
    using ValueType = Type;

    virtual Type operator()() = 0;
    virtual Type min() const = 0;
    virtual Type max() const = 0;
};
