#pragma once

#include "Image2D.h"
#include "GameData.h"
#include "Timer.h"
#include <string>
#include <fmt/core.h>

class Renderer {
 private:
  static constexpr int kFontSize = 30;

  Timer _timer;
  float _time = 0.f;
  float ballCurrentRotation = 0.f;

 public:
  Image2D ball;
  Image2D ground;
  Image2D goal_left;
  Image2D goal_right;
  Image2D player_blue;
  Image2D player_blue_left_feet;
  Image2D player_blue_right_feet;
  Image2D player_red;

  void Setup();

  void TearDown();

  void Draw(void);

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