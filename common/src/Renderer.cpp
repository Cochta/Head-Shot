#include "Renderer.h"

void Renderer::Setup() {
  ball.Setup("data/ball.png", 5, Offset::Center);
  goalLeft.Setup("data/porta.png", 5, Offset::DownLeft);
  goalRight.Setup("data/porta.png", 5, Offset::DownLeft);

  ground.Setup("data/terreno.png", 5, Offset::DownLeft);
}

void Renderer::TearDown() {
  ball.TearDown();
  goalLeft.TearDown();
  goalRight.TearDown();
  ground.TearDown();

  CloseWindow();
}

void Renderer::Draw(void) {
  BeginDrawing();
  {
    ClearBackground(SKYBLUE);

    ball.Draw({screen.width / 2, screen.height / 2});

    for (size_t i = 0; i < 8; i++) {
      ground.Draw({ground.Dest.width * i, screen.height});
    }
    goalLeft.Draw({0, screen.height - ground.Dest.height});
    goalRight.Draw({screen.width, screen.height - ground.Dest.height});

    const char* text = "Rollback soccer";

    DrawText(text, Metrics::Width * 0.5 - MeasureText(text, font_size) * 0.5,
             240, 30, WHITE);
  }
  EndDrawing();
}
