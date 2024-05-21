#pragma once
#include <array>
#include <string>
#include <stdexcept>

template <typename T>
using BoardType = std::array<std::array<T, 8>, 8>;

#define todo(msg) throw std::runtime_error(std::string("TODO: ") + msg);