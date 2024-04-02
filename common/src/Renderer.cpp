#include "Renderer.h"

void Renderer::Setup() {
  ball.Setup("data/ball.png", 5, Offset::Center);
  goalLeft.Setup("data/portaLeft.png", 5, Offset::DownLeft);
  goalRight.Setup("data/portaRight.png", 5, Offset::DownRight);
  ground.Setup("data/terreno.png", 5, Offset::DownCenter);
  Metrics::Groundheight = ground.Dest.height;
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

    ball.Draw({Metrics::Width / 2, Metrics::Height / 2});

    for (size_t i = 0; i < 8; i++) {
      ground.Draw({ground.Dest.width * i, Metrics::Height});
    }
    goalLeft.Draw({0, Metrics::Height - Metrics::Groundheight});
    goalRight.Draw({Metrics::Width, Metrics::Height - Metrics::Groundheight});
    

    const char* text = "Rollback soccer";

    DrawText(text, Metrics::Width * 0.5f - MeasureText(text, font_size) * 0.5f,
             240, 30, WHITE);
  }
  EndDrawing();
}
