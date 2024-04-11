#pragma once

#include <fmt/core.h>

#include <string>

#include "Game.h"
#include "GameData.h"
#include "Image2D.h"
#include "Timer.h"
enum class RenderState { kMenu, kStartGame, kInGame };

class Renderer {
 private:
  RenderState state_ = RenderState::kMenu;

  Game* game_;
  static constexpr int kFontSize = 30;

  Timer rotation_timer_;
  float rotation_time_ = 0.f;
  float ballCurrentRotation = 0.f;

  float game_time_ = 0.f;

 public:
  Image2D ball;
  Image2D ground;
  Image2D goal_left;
  Image2D goal_right;
  Image2D player_blue;
  Image2D player_blue_left_feet;
  Image2D player_blue_right_feet;
  Image2D player_red;

  void StartGame();

  void Setup(Game* game);

  void TearDown();

  void Draw(void);

  void SetGameTime(float time);

  void ChangeState(RenderState newState) noexcept;

 private:
  void SetupBall();
  void SetupPlayers();

  void DrawMenu();

  void DrawTimer();

  void DrawBall();
  void DrawPlayers();
  void DrawTerrain();
  void DrawHitbox();
};