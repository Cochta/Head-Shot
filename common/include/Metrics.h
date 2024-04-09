#pragma once

#include "Vec2.h"

namespace metrics  // Meter in physical world != meter irl
{

constexpr float MeterRatio = 100.f;

[[nodiscard]] constexpr float PixelsToMeters(float pixels) noexcept {
  return pixels / MeterRatio;
}

[[nodiscard]] constexpr float MetersToPixels(float meters) noexcept {
  return meters * MeterRatio;
}

constexpr int Width = MetersToPixels(12);
constexpr int Height = MetersToPixels(8);

inline float GroundHeight = 0.f;
inline Math::Vec2F GoalSize = {0.f, 0.f};
inline float BallRadius = 1.f;
inline float PlayerRadius = 1.f;

inline float FeetHeight = 0.f;

}  // namespace metrics
