#include "Renderer.h"

void Renderer::Setup() {
  ball.Setup("data/ball.png", 5, Offset::Center);
  Metrics::BallRadius = ball.dest.width * 0.5f;
  goal_left.Setup("data/portaLeft.png", 5, Offset::DownLeft);
  goal_right.Setup("data/portaRight.png", 5, Offset::DownRight);
  ground.Setup("data/terreno.png", 5, Offset::DownCenter);
  Metrics::GroundHeight = ground.dest.height;
  player_blue.Setup("data/PlayerBlu.png", 5, Offset::Center);
  Metrics::PlayerRadius = player_blue.dest.width * 0.5f;
  player_red.Setup("data/PlayerRed.png", 5, Offset::Center);
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

    ball.Draw({Metrics::BallPos.X, Metrics::BallPos.Y});

    for (size_t i = 0; i < 8; i++) {
      ground.Draw({ground.dest.width * i, Metrics::Height});
    }

    player_blue.Draw({Metrics::PlayerBluePos.X, Metrics::PlayerBluePos.Y});
    player_red.Draw({Metrics::PlayerRedPos.X, Metrics::PlayerRedPos.Y});

    goal_left.Draw({0, Metrics::Height - Metrics::GroundHeight});
    goal_right.Draw({Metrics::Width, Metrics::Height - Metrics::GroundHeight});

    DrawDebug();

    const char* text = "Rollback soccer";

    DrawText(text, Metrics::Width * 0.5f - MeasureText(text, kFontSize) * 0.5f,
             240, 30, WHITE);
  }
  EndDrawing();
}

void Renderer::DrawDebug() {
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
}
