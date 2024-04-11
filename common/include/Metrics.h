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

constexpr int kFPS = 60;

constexpr int kWindowWidth = MetersToPixels(12);
constexpr int kWindowHeight = MetersToPixels(8);

constexpr float kBallRadiusSmall = MetersToPixels(0.2f);
constexpr float kBallRadiusMedium = MetersToPixels(0.3f);
constexpr float kBallRadiusLarge = MetersToPixels(0.4f);

constexpr Math::Vec2F kGroundSize{kWindowWidth / 7.f, kWindowHeight / 5.f};
constexpr Math::Vec2F kGoalSize = {MetersToPixels(0.6f), kWindowHeight / 3.f};
constexpr float kPlayerRadius = MetersToPixels(0.5f);

}  // namespace metrics
