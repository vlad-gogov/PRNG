#pragma once

#include <stdexcept>
#include <string>

class BaseError : public std::exception {
    std::string what_str;

  public:
    BaseError(const std::string &message);
    ~BaseError() = default;

    virtual const char *what() const noexcept;
};