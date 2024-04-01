#include "Renderer.h"

void Image2D::Setup(const char* path) {
  Tex = LoadTexture(path);
  Source = {0, 0, static_cast<float>(Tex.width),
            static_cast<float>(Tex.height)};
}

void Image2D::TearDown() { UnloadTexture(Tex); }

void Image2D::Draw(Vector2 position, float scale) {
  Center = {Tex.width * scale * 0.5f, Tex.height * scale * 0.5f};
  Dest = {position.x, position.y, Source.width*scale, Source.height*scale};
  DrawTexturePro(Tex, Source, Dest, Center, 0, WHITE);
}

void Renderer::Setup() {
  penguin.Setup("data/penguin.png");
  InitAudioDevice();
}

void Renderer::TearDown() {
  penguin.TearDown();

  CloseAudioDevice();
  CloseWindow();
}

void Renderer::Draw(void) {
  BeginDrawing();
  {
    ClearBackground(BLACK);

    DrawLine(screen.width / 2, 0, screen.width / 2, screen.height, RED);
    DrawLine(0, screen.height / 2, screen.width, screen.height / 2, RED);
    penguin.Draw({screen.width / 2, screen.height / 2}, 8);

    const char* text = "Raylib sample !!!";

    DrawText(text, Metrics::Width * 0.5 - MeasureText(text, font_size) * 0.5,
             240, 30, WHITE);
  }
  EndDrawing();
}
