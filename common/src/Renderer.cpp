#include "Renderer.h"

void Renderer::StartGame() {
  SetupBall();
  SetupPlayers();
}

void Renderer::Setup(Game* game) {
  game_ = game;
  rotation_timer_.SetUp();
  goal_left.Setup("data/portaLeft.png",
                  {metrics::kGoalSize.X, metrics::kGoalSize.Y},
                  Offset::DownLeft);
  goal_right.Setup("data/portaRight.png",
                   {metrics::kGoalSize.X, metrics::kGoalSize.Y},
                   Offset::DownRight);

  ground.Setup("data/terreno.png",
               {metrics::kGroundSize.X, metrics::kGroundSize.Y},
               Offset::DownCenter);
}

void Renderer::TearDown() {
  ball.TearDown();
  goal_left.TearDown();
  goal_right.TearDown();
  ground.TearDown();
  player_blue.TearDown();
  player_red.TearDown();

  CloseWindow();
}

void Renderer::Draw(void) {
  BeginDrawing();
  {
    ClearBackground(SKYBLUE);
    DrawFPS(5, 5);

    rotation_timer_.Tick();
    rotation_time_ += rotation_timer_.DeltaTime;

    switch (state_) {
      case RenderState::kMenu:
        DrawMenu();
        break;
      case RenderState::kStartGame:
        game_->StartGame();
        StartGame();
        state_ = RenderState::kInGame;
        break;
      case RenderState::kInGame:
        DrawBall();
        DrawPlayers();
        DrawTimer();
        break;
    }

    DrawTerrain();

    // DrawHitbox();
  }
  EndDrawing();
}

void Renderer::SetGameTime(float time) { game_time_ = time; }

void Renderer::ChangeState(RenderState newState) noexcept { state_ = newState; }

void Renderer::SetupBall() {
  const Vector2 ballSize{game_->GetBallRadius() * 2,
                         game_->GetBallRadius() * 2};
  switch (game_->GetBallType()) {
    case BallType::kFootball:
      ball.Setup("data/football.png", ballSize, Offset::Center);
      break;
    case BallType::kVolleyball:
      ball.Setup("data/volley-ball.png", ballSize, Offset::Center);
      break;
    case BallType::kBasketball:
      ball.Setup("data/basket-ball.png", ballSize, Offset::Center);
      break;
    case BallType::kTennisball:
      ball.Setup("data/tennis-ball.png", ballSize, Offset::Center);
      break;
    case BallType::kBaseball:
      ball.Setup("data/base-ball.png", ballSize, Offset::Center);
      break;
  }
}

void Renderer::SetupPlayers() {
  // player blue
  player_blue.Setup(
      "data/PlayerBlu.png",
      {metrics::kPlayerRadius * 2.f, metrics::kPlayerRadius * 2.f},
      Offset::Center);

  // player_blue_left_feet.Setup("data/PlayerBluLeftLeg.png", 5.f,
  //                             Offset::DownRight);
  // player_blue_right_feet.Setup("data/PlayerBluRightLeg.png", 5.f,
  //                              Offset::DownLeft);
  // metrics::FeetHeight = player_blue_left_feet.dest.height;

  // player red
  player_red.Setup("data/PlayerRed.png",
                   {metrics::kPlayerRadius * 2.f, metrics::kPlayerRadius * 2.f},
                   Offset::Center);
}

void Renderer::DrawMenu() {
  const char* text = "Start Game";
  Rectangle startRect = {
      metrics::kWindowWidth * 0.333f, metrics::kWindowHeight * 0.2f,
      metrics::kWindowWidth * 0.333f, metrics::kWindowHeight * 0.1f};
  Color color = WHITE;

  if (CheckCollisionPointRec(GetMousePosition(), startRect)) {
    color = YELLOW;
    if (IsMouseButtonPressed(0)) {
      state_ = RenderState::kStartGame;
      // todo: send to network manager and the network manager starts the game
    }
  }

  DrawRectangleRec(startRect, color);

  DrawRaylibText(
      text, metrics::kWindowWidth * 0.5f - MeasureText(text, kFontSize) * 0.5f,
      metrics::kWindowHeight * 0.2f + 30, 30, BLACK);
}

void Renderer::DrawTimer() {
  int totalTime = 90 - game_time_;
  int minutes = totalTime / 60;
  int seconds = totalTime % 60;

  std::string formattedTime = fmt::format("{:02d}:{:02d}", minutes, seconds);

  int textSize = kFontSize * 2;

  DrawRaylibText(formattedTime.c_str(),
                 metrics::kWindowWidth * 0.5f -
                     MeasureText(formattedTime.c_str(), textSize) * 0.5f,
                 metrics::kWindowHeight * 0.2f + 30, textSize, BLACK);
}

void Renderer::DrawBall() {
  const float distanceMovedThisFrame =
      rotation_timer_.DeltaTime * game_->GetBallVelocity().X;

  const float rotationRadiansThisFrame =
      distanceMovedThisFrame / game_->GetBallRadius();

  const float rotationDegreesThisFrame =
      rotationRadiansThisFrame * (180.0f / Math::Pi);

  ballCurrentRotation += rotationDegreesThisFrame;

  ballCurrentRotation = fmod(ballCurrentRotation, 360.0f);
  if (ballCurrentRotation < 0) {
    ballCurrentRotation += 360.0f;  // Correct negative rotation
  }
  const auto ballPos = game_->GetBallPosition();
  ball.Draw({ballPos.X, ballPos.Y}, ballCurrentRotation);
}

void Renderer::DrawPlayers() {
  const auto playerBluePos = game_->GetPlayerBluePos();
  const auto playerRedPos = game_->GetPlayerRedPos();

  player_blue.Draw({playerBluePos.X, playerBluePos.Y});

  player_red.Draw({playerRedPos.X, playerRedPos.Y});

  // player_blue_left_feet.Draw(
  //     {game_data::PlayerBluePos.X,
  //      game_data::PlayerBluePos.Y + metrics::kPlayerRadius});

  // player_blue_right_feet.Draw(
  //     {game_data::PlayerBluePos.X,
  //      game_data::PlayerBluePos.Y + metrics::kPlayerRadius});
}

void Renderer::DrawTerrain() {
  for (size_t i = 0; i < 8; i++) {
    ground.Draw({ground.dest.width * i, metrics::kWindowHeight});
  }

  goal_left.Draw({0, metrics::kWindowHeight - metrics::kGroundSize.Y});
  goal_right.Draw(
      {metrics::kWindowWidth, metrics::kWindowHeight - metrics::kGroundSize.Y});
}

void Renderer::DrawHitbox() {
  const auto ballPos = game_->GetBallPosition();
  DrawCircle(ballPos.X, ballPos.Y, game_->GetBallRadius(),
             WHITE);  // ball

  DrawRectangle(0, metrics::kWindowHeight - metrics::kGroundSize.Y,
                metrics::kWindowWidth, metrics::kGroundSize.Y,
                GREEN);  // ground

  const auto playerBluePos = game_->GetPlayerBluePos();
  const auto playerRedPos = game_->GetPlayerRedPos();

  DrawCircle(playerBluePos.X, playerBluePos.Y, metrics::kPlayerRadius,
             BLUE);  // player blue

  DrawCircle(playerRedPos.X, playerRedPos.Y, metrics::kPlayerRadius,
             RED);  // player red

  DrawRectangle(
      0, metrics::kWindowHeight - metrics::kGroundSize.Y - metrics::kGoalSize.Y,
      metrics::kGoalSize.X, metrics::kGoalSize.Y / 10.f, WHITE);  // goal left

  DrawRectangle(
      metrics::kWindowWidth - metrics::kGoalSize.X,
      metrics::kWindowHeight - metrics::kGroundSize.Y - metrics::kGoalSize.Y,
      metrics::kGoalSize.X, metrics::kGoalSize.Y / 10.f,
      WHITE);  // goal right
}
