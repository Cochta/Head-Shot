#include "Renderer.h"

void Renderer::Setup() {
  _timer.SetUp();
  SetupBall();
  goal_left.Setup("data/portaLeft.png", 5, Offset::DownLeft);
  Metrics::GoalSize = {goal_left.dest.width, goal_left.dest.height};
  goal_right.Setup("data/portaRight.png", 5, Offset::DownRight);
  ground.Setup("data/terreno.png", 5, Offset::DownCenter);
  Metrics::GroundHeight = ground.dest.height;

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

    switch (Metrics::State) {
      case Metrics::GameState::MENU:
        DrawMenu();
        break;
      case Metrics::GameState::STARTGAME:
        break;
      case Metrics::GameState::INGAME:
        DrawBall();
        DrawPlayers();
        break;
    }

    DrawTerrain();

    // DrawHitbox();
  }
  EndDrawing();
}

void Renderer::SetupBall() {
  switch (Metrics::BallType) {
    case Metrics::BallType::FOOTBALL:
      ball.Setup("data/football.png", 0.055f, Offset::Center);
      break;
    case Metrics::BallType::VOLLEYBALL:
      ball.Setup("data/volley-ball.png", 0.055f, Offset::Center);
      break;
    case Metrics::BallType::BASKETBALL:
      ball.Setup("data/basket-ball.png", 0.055f, Offset::Center);
      break;
    case Metrics::BallType::TENNISBALL:
      ball.Setup("data/tennis-ball.png", 0.055f, Offset::Center);
      break;
    case Metrics::BallType::BASEBALL:
      ball.Setup("data/base-ball.png", 0.055f, Offset::Center);
      break;
  }

  Metrics::BallRadius = ball.dest.width * 0.5f;
}

void Renderer::SetupPlayers() {
  // player blue
  player_blue.Setup("data/PlayerBlu.png", 5.f, Offset::Center);

  player_blue_left_feet.Setup("data/PlayerBluLeftLeg.png", 5.f,
                              Offset::DownRight);
  player_blue_right_feet.Setup("data/PlayerBluRightLeg.png", 5.f,
                               Offset::DownLeft);
  Metrics::FeetHeight = player_blue_left_feet.dest.height;
  Metrics::PlayerRadius = player_blue.dest.width * 0.5f;

  // player red
  player_red.Setup("data/PlayerRed.png", 5.f, Offset::Center);
}

void Renderer::DrawMenu() {
  const char* text = "Start Game";
  Rectangle startRect = {Metrics::Width * 0.333f, Metrics::Height * 0.2f,
                         Metrics::Width * 0.333f, Metrics::Height * 0.1f};
  Color color = WHITE;

  if (CheckCollisionPointRec(GetMousePosition(), startRect)) {
    color = YELLOW;
    if (IsMouseButtonPressed(0)) {
      Metrics::State = Metrics::GameState::STARTGAME;
    }
  }
  

  DrawRectangleRec(startRect, color);

  DrawText(text, Metrics::Width * 0.5f - MeasureText(text, kFontSize) * 0.5f,
           Metrics::Height * 0.2f + 30, 30, BLACK);
}

void Renderer::DrawBall() {
  float distanceMovedThisFrame = _timer.DeltaTime * Metrics::BallVelocity.X;

  float rotationRadiansThisFrame = distanceMovedThisFrame / Metrics::BallRadius;

  float rotationDegreesThisFrame =
      rotationRadiansThisFrame * (180.0f / Math::Pi);

  ballCurrentRotation += rotationDegreesThisFrame;

  ballCurrentRotation = fmod(ballCurrentRotation, 360.0f);
  if (ballCurrentRotation < 0) {
    ballCurrentRotation += 360.0f;  // Correct negative rotation
  }

  ball.Draw({Metrics::BallPos.X, Metrics::BallPos.Y}, ballCurrentRotation);
}

void Renderer::DrawPlayers() {
  player_blue.Draw({Metrics::PlayerBluePos.X, Metrics::PlayerBluePos.Y});
  player_blue_left_feet.Draw(
      {Metrics::PlayerBluePos.X,
       Metrics::PlayerBluePos.Y + Metrics::PlayerRadius});
  player_blue_right_feet.Draw(
      {Metrics::PlayerBluePos.X,
       Metrics::PlayerBluePos.Y + Metrics::PlayerRadius});
  player_red.Draw({Metrics::PlayerRedPos.X, Metrics::PlayerRedPos.Y});
}

void Renderer::DrawTerrain() {
  for (size_t i = 0; i < 8; i++) {
    ground.Draw({ground.dest.width * i, Metrics::Height});
  }

  goal_left.Draw({0, Metrics::Height - Metrics::GroundHeight});
  goal_right.Draw({Metrics::Width, Metrics::Height - Metrics::GroundHeight});
}

void Renderer::DrawHitbox() {
  DrawCircle(Metrics::BallPos.X, Metrics::BallPos.Y, Metrics::BallRadius,
             WHITE);  // ball

  DrawRectangle(0, Metrics::Height - Metrics::GroundHeight, Metrics::Width,
                Metrics::GroundHeight, GREEN);  // ground

  DrawCircle(Metrics::PlayerBluePos.X, Metrics::PlayerBluePos.Y,
             Metrics::PlayerRadius,
             BLUE);  // player blue

  DrawCircle(Metrics::PlayerRedPos.X, Metrics::PlayerRedPos.Y,
             Metrics::PlayerRadius,
             RED);  // player red

  DrawCircle(0, Metrics::Height - Metrics::GroundHeight - Metrics::GoalSize.Y,
             Metrics::GoalSize.X, WHITE);  // goal left

  DrawCircle(Metrics::Width,
             Metrics::Height - Metrics::GroundHeight - Metrics::GoalSize.Y,
             Metrics::GoalSize.X, WHITE);  // goal left
}
