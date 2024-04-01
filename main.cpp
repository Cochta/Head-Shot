#include "Audio.h"
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

  PlaySound(audio.music);

#ifdef PLATFORM_WEB
  emscripten_set_main_loop_arg(UpdateDrawFrame, &renderer, 0, 1);

#else

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    renderer.Draw();
  }

#endif

  renderer.TearDown();
  audio.TearDown();

  return EXIT_SUCCESS;
}