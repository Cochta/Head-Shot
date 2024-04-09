#pragma once
#include "Input.h"
#include "Metrics.h"
#include "Timer.h"
#include "Vec2.h"

namespace game_data {
enum class BallType { FOOTBALL, VOLLEYBALL, BASKETBALL, TENNISBALL, BASEBALL };

enum class GameState { MENU, STARTGAME, INGAME };

inline BallType Type;

inline GameState State = GameState::MENU;

inline Timer GameTimer;
inline float GameTime = 0;
inline int GameFrame = 0;

inline Math::Vec2F BallPos = {metrics::Width * 0.5f, metrics::Height * 0.5f};
inline Math::Vec2F BallVelocity = {0.f, 0.f};
inline Math::Vec2F PlayerBluePos = {metrics::Width * 0.33f,
                                    metrics::Height * 0.66f};
inline Math::Vec2F PlayerRedPos = {metrics::Width - metrics::Width * 0.33f,
                                   metrics::Height * 0.66f};

inline std::array<input::Input, 5400> GameInputs;

}  // namespace game_data