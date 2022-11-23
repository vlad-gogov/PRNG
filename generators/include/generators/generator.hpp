#pragma once

template <class Type>
class Generator {

public:
    virtual Type operator()() = 0;
    virtual Type min() = 0;
    virtual Type max() = 0;
};
