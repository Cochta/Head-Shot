#include "Audio.h"
#include "Game.h"
#include "Renderer.h"

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#else
#endif

#include <iostream>

// Update and Draw one frame
void UpdateDrawFrame(void* renderer) {
  static_cast<Renderer*>(renderer)->Draw();
}

int main() {
  InitWindow(Metrics::Width, Metrics::Height, "Raylib sample");

  Renderer renderer;

  renderer.Setup();

  Audio audio;
  audio.Setup();

  Game game;
  game.Setup();

  PlaySound(audio.music);

#ifdef PLATFORM_WEB
  emscripten_set_main_loop_arg(UpdateDrawFrame, &renderer, 0, 1);

#else

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    if (IsKeyDown(KEY_D)) {
      game.ProcessInput(Input::Right);
    }
    if (IsKeyDown(KEY_A)) {
      game.ProcessInput(Input::Left);
    }
    if (IsKeyPressed(KEY_SPACE)) {
      game.ProcessInput(Input::Jump);
    }
    game.Update();
    renderer.Draw();
  }

#endif

  renderer.TearDown();
  audio.TearDown();
  game.TearDown();

  return EXIT_SUCCESS;
}