#include "Renderer.h"

void Renderer::Setup() {
  _timer.SetUp();
  SetupBall();
  goal_left.Setup("data/portaLeft.png", 5, Offset::DownLeft);
  metrics::GoalSize = {goal_left.dest.width, goal_left.dest.height};
  goal_right.Setup("data/portaRight.png", 5, Offset::DownRight);
  ground.Setup("data/terreno.png", 5, Offset::DownCenter);
  metrics::GroundHeight = ground.dest.height;

  SetupPlayers();
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

    _timer.Tick();
    _time += _timer.DeltaTime;

    switch (game_data::State) {
      case game_data::GameState::MENU:
        DrawMenu();
        break;
      case game_data::GameState::STARTGAME:
        break;
      case game_data::GameState::INGAME:
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

void Renderer::SetupBall() {
  switch (game_data::Type) {
    case game_data::BallType::FOOTBALL:
      ball.Setup("data/football.png", 0.055f, Offset::Center);
      break;
    case game_data::BallType::VOLLEYBALL:
      ball.Setup("data/volley-ball.png", 0.055f, Offset::Center);
      break;
    case game_data::BallType::BASKETBALL:
      ball.Setup("data/basket-ball.png", 0.055f, Offset::Center);
      break;
    case game_data::BallType::TENNISBALL:
      ball.Setup("data/tennis-ball.png", 0.055f, Offset::Center);
      break;
    case game_data::BallType::BASEBALL:
      ball.Setup("data/base-ball.png", 0.055f, Offset::Center);
      break;
  }

  metrics::BallRadius = ball.dest.width * 0.5f;
}

void Renderer::SetupPlayers() {
  // player blue
  player_blue.Setup("data/PlayerBlu.png", 5.f, Offset::Center);

  player_blue_left_feet.Setup("data/PlayerBluLeftLeg.png", 5.f,
                              Offset::DownRight);
  player_blue_right_feet.Setup("data/PlayerBluRightLeg.png", 5.f,
                               Offset::DownLeft);
  metrics::FeetHeight = player_blue_left_feet.dest.height;
  metrics::PlayerRadius = player_blue.dest.width * 0.5f;

  // player red
  player_red.Setup("data/PlayerRed.png", 5.f, Offset::Center);
}

void Renderer::DrawMenu() {
  const char* text = "Start Game";
  Rectangle startRect = {metrics::Width * 0.333f, metrics::Height * 0.2f,
                         metrics::Width * 0.333f, metrics::Height * 0.1f};
  Color color = WHITE;

  if (CheckCollisionPointRec(GetMousePosition(), startRect)) {
    color = YELLOW;
    if (IsMouseButtonPressed(0)) {
      game_data::State = game_data::GameState::STARTGAME;
    }
  }

  DrawRectangleRec(startRect, color);

  DrawText(text, metrics::Width * 0.5f - MeasureText(text, kFontSize) * 0.5f,
           metrics::Height * 0.2f + 30, 30, BLACK);
}

void Renderer::DrawTimer() {
  int totalTime = 90 - game_data::GameTime;
  int minutes = totalTime / 60;
  int seconds = totalTime % 60;

  std::string formattedTime = fmt::format("{:02d}:{:02d}", minutes, seconds);
  
  int textSize = kFontSize * 2;

  DrawText(formattedTime.c_str(),
           metrics::Width * 0.5f -
               MeasureText(formattedTime.c_str(), textSize) * 0.5f,
           metrics::Height * 0.2f + 30, textSize, BLACK);
}

void Renderer::DrawBall() {
  float distanceMovedThisFrame = _timer.DeltaTime * game_data::BallVelocity.X;

  float rotationRadiansThisFrame = distanceMovedThisFrame / metrics::BallRadius;

  float rotationDegreesThisFrame =
      rotationRadiansThisFrame * (180.0f / Math::Pi);

  ballCurrentRotation += rotationDegreesThisFrame;

  ballCurrentRotation = fmod(ballCurrentRotation, 360.0f);
  if (ballCurrentRotation < 0) {
    ballCurrentRotation += 360.0f;  // Correct negative rotation
  }

  ball.Draw({game_data::BallPos.X, game_data::BallPos.Y}, ballCurrentRotation);
}

void Renderer::DrawPlayers() {
  player_blue.Draw({game_data::PlayerBluePos.X, game_data::PlayerBluePos.Y});
  player_blue_left_feet.Draw(
      {game_data::PlayerBluePos.X,
       game_data::PlayerBluePos.Y + metrics::PlayerRadius});
  player_blue_right_feet.Draw(
      {game_data::PlayerBluePos.X,
       game_data::PlayerBluePos.Y + metrics::PlayerRadius});
  player_red.Draw({game_data::PlayerRedPos.X, game_data::PlayerRedPos.Y});
}

void Renderer::DrawTerrain() {
  for (size_t i = 0; i < 8; i++) {
    ground.Draw({ground.dest.width * i, metrics::Height});
  }

  goal_left.Draw({0, metrics::Height - metrics::GroundHeight});
  goal_right.Draw({metrics::Width, metrics::Height - metrics::GroundHeight});
}

void Renderer::DrawHitbox() {
  DrawCircle(game_data::BallPos.X, game_data::BallPos.Y, metrics::BallRadius,
             WHITE);  // ball

  DrawRectangle(0, metrics::Height - metrics::GroundHeight, metrics::Width,
                metrics::GroundHeight, GREEN);  // ground

  DrawCircle(game_data::PlayerBluePos.X, game_data::PlayerBluePos.Y,
             metrics::PlayerRadius,
             BLUE);  // player blue

  DrawCircle(game_data::PlayerRedPos.X, game_data::PlayerRedPos.Y,
             metrics::PlayerRadius,
             RED);  // player red

  DrawCircle(0, metrics::Height - metrics::GroundHeight - metrics::GoalSize.Y,
             metrics::GoalSize.X, WHITE);  // goal left

  DrawCircle(metrics::Width,
             metrics::Height - metrics::GroundHeight - metrics::GoalSize.Y,
             metrics::GoalSize.X, WHITE);  // goal left
}
