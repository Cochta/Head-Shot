#pragma once

#include <cstdint>

namespace input {

using Input = std::uint8_t;

constexpr Input kJump = 1 << 0;
constexpr Input kLeft = 1 << 1;
constexpr Input kRight = 1 << 2;
constexpr Input kKick = 1 << 3;

struct FrameInput {
  Input input = 0;
  short frame_nbr = 0;
};

}  // namespace input