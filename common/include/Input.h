#pragma once

#include <cstdint>

namespace input {

using Input = std::uint8_t;

constexpr Input kJump = 1 << 0;
constexpr Input kLeft = 1 << 1;
constexpr Input kRight = 1 << 2;
constexpr Input kKick = 1 << 3;

}  // namespace input