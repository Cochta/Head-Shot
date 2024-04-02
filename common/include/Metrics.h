#pragma once

namespace Metrics  // Meter in physical world != meter irl
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

inline float Groundheight = 0.f;
inline Vector2 BallPos = {0, 0};
}  // namespace Metrics
