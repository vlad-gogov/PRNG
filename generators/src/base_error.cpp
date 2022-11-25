#include "base_error.hpp"

BaseError::BaseError(const std::string &message) : what_str(message) {}

const char *BaseError::what() const noexcept {
    return what_str.c_str();
}
