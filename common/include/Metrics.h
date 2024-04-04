#pragma once

#include "Vec2.h"

namespace Metrics  // Meter in physical world != meter irl
{
enum class BallType { FOOTBALL, VOLLEYBALL, BASKETBALL, TENNISBALL, BASEBALL };

enum class GameState { MENU, STARTGAME, INGAME };

constexpr float MeterRatio = 100.f;

[[nodiscard]] constexpr float PixelsToMeters(float pixels) noexcept {
  return pixels / MeterRatio;
}

[[nodiscard]] constexpr float MetersToPixels(float meters) noexcept {
  return meters * MeterRatio;
}

constexpr int Width = MetersToPixels(12);
constexpr int Height = MetersToPixels(8);

inline BallType BallType;

inline GameState State = GameState::MENU;

inline float GroundHeight = 0.f;
inline Math::Vec2F GoalSize = {0.f, 0.f};
inline float BallRadius = 1.f;
inline float PlayerRadius = 1.f;

inline float FeetHeight = 0, f;

inline Math::Vec2F BallPos = {Width * 0.5f, Height * 0.5f};
inline Math::Vec2F BallVelocity = {0.f, 0.f};
inline Math::Vec2F PlayerBluePos = {Width * 0.33f, Height * 0.66f};
inline Math::Vec2F PlayerRedPos = {Width - Width * 0.33f, Height * 0.66f};

}  // namespace Metrics
