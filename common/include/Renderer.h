#pragma once

#include <fmt/core.h>

#include <string>

#include "Game.h"
#include "Image2D.h"
#include "Timer.h"

class Network;

class Renderer {
 private:
  Game* game_{};
  Network* network_{};

  static constexpr int kFontSize = 30;

  Timer rotation_timer_;
  float rotation_time_ = 0.f;
  float ball_current_rotation_ = 0.f;

  float game_time_ = 0.f;

  raylib::Rectangle start_btn_rect = {
      metrics::kWindowWidth * 0.333f, metrics::kWindowHeight * 0.2f,
      metrics::kWindowWidth * 0.333f, metrics::kWindowHeight * 0.1f};
  raylib::Color start_btn_text_color_ = raylib::WHITE;

  Image2D ball_{};
  Image2D ground_{};
  Image2D goal_left_{};
  Image2D goal_right_{};
  Image2D player_blue_{};
  Image2D player_blue_left_feet_{};
  Image2D player_blue_right_feet_{};
  Image2D player_red_{};
  Image2D player_red_left_feet_{};
  Image2D player_red_right_feet_{};

  bool isWaitingOtherPlayer = false;

 public:
  void StartGame();

  void Setup(Game* game, Network* network);

  void TearDown();

  void Draw(void);

  void SetGameTime(float time);

 private:
  void SetupBall();
  void SetupPlayers();

  void DrawMenu();
  void DrawScore();

  void DrawEndGame();

  void DrawTimer();

  void DrawBall();
  void DrawPlayers();
  void DrawTerrain();
  void DrawHitbox();
};