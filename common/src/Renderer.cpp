#include "Renderer.h"

#include "network.h"

void Renderer::StartGame() {
  SetupBall();
  SetupPlayers();
}

void Renderer::Setup(Game* game, Network* network) {
  game_ = game;
  network_ = network;
  rotation_timer_.SetUp();
  goal_left_.Setup("data/portaLeft.png",
                   {metrics::kGoalSize.X, metrics::kGoalSize.Y},
                   Offset::DownLeft);
  goal_right_.Setup("data/portaRight.png",
                    {metrics::kGoalSize.X, metrics::kGoalSize.Y},
                    Offset::DownRight);

  ground_.Setup("data/terreno.png",
                {metrics::kGroundSize.X, metrics::kGroundSize.Y},
                Offset::DownCenter);
}

void Renderer::TearDown() {
  ball_.TearDown();
  goal_left_.TearDown();
  goal_right_.TearDown();
  ground_.TearDown();
  player_blue_.TearDown();
  player_red_.TearDown();

  raylib::CloseWindow();
}

void Renderer::Draw(void) {
  raylib::BeginDrawing();
  {
    raylib::ClearBackground(raylib::SKYBLUE);
    raylib::DrawFPS(5, 5);

    rotation_timer_.Tick();
    rotation_time_ += rotation_timer_.DeltaTime;

    DrawTerrain();

    // DrawHitbox();

    switch (game_->GetState()) {
      case GameState::kMenu:
        DrawMenu();
        break;

      case GameState::kInGame:
        DrawBall();
        DrawScore();
        DrawPlayers();
        DrawTimer();
        break;
      case GameState::kGameFinished:
        DrawEndGame();
        break;
    }
  }
  raylib::EndDrawing();
}

void Renderer::SetGameTime(float time) { game_time_ = time; }

void Renderer::SetupBall() {
  const raylib::Vector2 ballSize{game_->GetBallRadius() * 2,
                                 game_->GetBallRadius() * 2};
  switch (game_->GetBallType()) {
    case BallType::kFootball:
      ball_.Setup("data/football.png", ballSize, Offset::Center);
      break;
    case BallType::kVolleyball:
      ball_.Setup("data/volley-ball.png", ballSize, Offset::Center);
      break;
    case BallType::kBasketball:
      ball_.Setup("data/basket-ball.png", ballSize, Offset::Center);
      break;
    case BallType::kTennisball:
      ball_.Setup("data/tennis-ball.png", ballSize, Offset::Center);
      break;
    case BallType::kBaseball:
      ball_.Setup("data/base-ball.png", ballSize, Offset::Center);
      break;
  }
}

void Renderer::SetupPlayers() {
  // player blue
  player_blue_.Setup(
      "data/PlayerBlu.png",
      {metrics::kPlayerRadius * 2.f, metrics::kPlayerRadius * 2.f},
      Offset::Center);

  player_blue_left_feet_.Setup(
      "data/PlayerBluLeftLeg.png",
      {metrics::kPlayerRadius, metrics::kPlayerRadius * 0.5f},
      Offset::DownRight);
  player_blue_right_feet_.Setup(
      "data/PlayerBluRightLeg.png",
      {metrics::kPlayerRadius, metrics::kPlayerRadius * 0.5f},
      Offset::DownLeft);

  // player red
  player_red_.Setup(
      "data/PlayerRed.png",
      {metrics::kPlayerRadius * 2.f, metrics::kPlayerRadius * 2.f},
      Offset::Center);

  player_red_left_feet_.Setup(
      "data/PlayerRedLeftLeg.png",
      {metrics::kPlayerRadius, metrics::kPlayerRadius * 0.5f},
      Offset::DownRight);
  player_red_right_feet_.Setup(
      "data/PlayerRedRightLeg.png",
      {metrics::kPlayerRadius, metrics::kPlayerRadius * 0.5f},
      Offset::DownLeft);
}

void Renderer::DrawMenu() {
  const char* text = "Start Game";
  start_btn_text_color_ = raylib::WHITE;
  if (!network_->IsConnected()) {
    text = "Waiting for connection";
  } else {
    if (isWaitingOtherPlayer) {
      text = "Waiting for other player";
    } else {
      if (CheckCollisionPointRec(raylib::GetMousePosition(), start_btn_rect)) {
        start_btn_text_color_ = raylib::YELLOW;
        if (raylib::IsMouseButtonPressed(0)) {
          isWaitingOtherPlayer = true;
          network_->JoinRandomOrCreateRoom();
        }
      }
    }
  }

  DrawRectangleRec(start_btn_rect, start_btn_text_color_);

  raylib::DrawRaylibText(text,
                         metrics::kWindowWidth * 0.5f -
                             raylib::MeasureText(text, kFontSize) * 0.5f,
                         metrics::kWindowHeight * 0.2f + 30, 30, raylib::BLACK);
}

void Renderer::DrawScore() {
  raylib::DrawRaylibText(
      std::to_string(game_->GetBlueScore()).c_str(),
      metrics::kWindowWidth * 0.33f -
          raylib::MeasureText(std::to_string(game_->GetBlueScore()).c_str(),
                              100) *
              0.5f,
      metrics::kWindowHeight * 0.33f, 100, raylib::BLUE);

  raylib::DrawRaylibText(
      std::to_string(game_->GetRedScore()).c_str(),
      metrics::kWindowWidth * 0.66f -
          raylib::MeasureText(std::to_string(game_->GetRedScore()).c_str(),
                              100) *
              0.5f,
      metrics::kWindowHeight * 0.33f, 100, raylib::RED);
}

void Renderer::DrawEndGame() {
  DrawBall();
  DrawScore();
  DrawPlayers();

  int blueScore = game_->GetBlueScore();
  int redScore = game_->GetRedScore();

  const char* text = "";
  auto color = raylib::BLACK;

  int textSize = kFontSize * 2;

  if (blueScore == redScore) {
    text = "Draw !";
  } else if (blueScore > redScore) {
    text = "Blue wins";
    color = raylib::BLUE;
  } else {
    text = "Red wins";
    color = raylib::RED;
  }

  raylib::DrawRaylibText(
      text,
      metrics::kWindowWidth * 0.5f - raylib::MeasureText(text, textSize) * 0.5f,
      30, textSize, color);

  text = "Main menu";
  start_btn_text_color_ = raylib::WHITE;

  if (CheckCollisionPointRec(raylib::GetMousePosition(), start_btn_rect)) {
    start_btn_text_color_ = raylib::YELLOW;
    if (raylib::IsMouseButtonPressed(0)) {
      game_->Restart();
      isWaitingOtherPlayer = false;
    }
  }

  DrawRectangleRec(start_btn_rect, start_btn_text_color_);

  raylib::DrawRaylibText(text,
                         metrics::kWindowWidth * 0.5f -
                             raylib::MeasureText(text, kFontSize) * 0.5f,
                         metrics::kWindowHeight * 0.2f + 30, 30, raylib::BLACK);
}

void Renderer::DrawTimer() {
  int totalTime = 90 - game_time_;
  int minutes = totalTime / 60;
  int seconds = totalTime % 60;

  std::string formattedTime = fmt::format("{:02d}:{:02d}", minutes, seconds);

  int textSize = kFontSize * 2;

  raylib::DrawRaylibText(
      formattedTime.c_str(),
      metrics::kWindowWidth * 0.5f -
          raylib::MeasureText(formattedTime.c_str(), textSize) * 0.5f,
      metrics::kWindowHeight * 0.2f + 30, textSize, raylib::BLACK);
}

void Renderer::DrawBall() {
  const float distanceMovedThisFrame =
      rotation_timer_.DeltaTime * game_->GetBallVelocity().X;

  const float rotationRadiansThisFrame =
      distanceMovedThisFrame / game_->GetBallRadius();

  const float rotationDegreesThisFrame =
      rotationRadiansThisFrame * (180.0f / Math::Pi);

  ball_current_rotation_ += rotationDegreesThisFrame;

  ball_current_rotation_ = fmod(ball_current_rotation_, 360.0f);
  if (ball_current_rotation_ < 0) {
    ball_current_rotation_ += 360.0f;  // Correct negative rotation
  }
  const auto ballPos = game_->GetBallPosition();
  ball_.Draw({ballPos.X, ballPos.Y}, ball_current_rotation_);
}

void Renderer::DrawPlayers() {
  const auto playerBluePos = game_->GetPlayerBluePos();
  const auto playerRedPos = game_->GetPlayerRedPos();

  player_blue_.Draw({playerBluePos.X, playerBluePos.Y});

  player_red_.Draw({playerRedPos.X, playerRedPos.Y});

  player_blue_left_feet_.Draw(
      {playerBluePos.X, playerBluePos.Y + metrics::kPlayerRadius +
                            player_blue_left_feet_.dest.height * 0.5f});

  player_blue_right_feet_.Draw(
      {playerBluePos.X, playerBluePos.Y + metrics::kPlayerRadius +
                            player_blue_right_feet_.dest.height * 0.5f});

  player_red_left_feet_.Draw(
      {playerRedPos.X, playerRedPos.Y + metrics::kPlayerRadius +
                           player_red_left_feet_.dest.height * 0.5f});

  player_red_right_feet_.Draw(
      {playerRedPos.X, playerRedPos.Y + metrics::kPlayerRadius +
                           player_red_right_feet_.dest.height * 0.5f});
}

void Renderer::DrawTerrain() {
  for (size_t i = 0; i < 8; i++) {
    ground_.Draw({ground_.dest.width * i, metrics::kWindowHeight});
  }

  goal_left_.Draw({0, metrics::kWindowHeight - metrics::kGroundSize.Y});
  goal_right_.Draw(
      {metrics::kWindowWidth, metrics::kWindowHeight - metrics::kGroundSize.Y});
}

void Renderer::DrawHitbox() {
  const auto ballPos = game_->GetBallPosition();
  raylib::DrawCircle(ballPos.X, ballPos.Y, game_->GetBallRadius(),
                     raylib::WHITE);  // ball

  raylib::DrawRectangle(0, metrics::kWindowHeight - metrics::kGroundSize.Y,
                        metrics::kWindowWidth, metrics::kGroundSize.Y,
                        raylib::GREEN);  // ground

  const auto playerBluePos = game_->GetPlayerBluePos();
  const auto playerRedPos = game_->GetPlayerRedPos();

  raylib::DrawCircle(playerBluePos.X, playerBluePos.Y, metrics::kPlayerRadius,
                     raylib::BLUE);  // player blue

  raylib::DrawCircle(playerRedPos.X, playerRedPos.Y, metrics::kPlayerRadius,
                     raylib::RED);  // player red

  raylib::DrawRectangle(
      0, metrics::kWindowHeight - metrics::kGroundSize.Y - metrics::kGoalSize.Y,
      metrics::kGoalSize.X, metrics::kGoalSize.Y / 10.f,
      raylib::WHITE);  // goal left

  raylib::DrawRectangle(
      metrics::kWindowWidth - metrics::kGoalSize.X,
      metrics::kWindowHeight - metrics::kGroundSize.Y - metrics::kGoalSize.Y,
      metrics::kGoalSize.X, metrics::kGoalSize.Y / 10.f,
      raylib::WHITE);  // goal right
}
