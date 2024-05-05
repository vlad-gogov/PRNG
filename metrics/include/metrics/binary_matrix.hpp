#pragma once
#include "metrics/utils.hpp"

class BinaryMatrix {

    utils::seq_bytes bytes;
    size_t size;

  public:
    BinaryMatrix();
    BinaryMatrix(const utils::seq_bytes bytes, size_t size);

    size_t compute_rank() const;

    void print() const;
};