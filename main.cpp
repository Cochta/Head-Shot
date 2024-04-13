#include "Audio.h"
#include "Game.h"
#include "Renderer.h"
#include "network.h"

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#else
#endif

#ifdef TRACY_ENABLE
#include <TracyC.h>
#include <Tracy.hpp>
#endif

#include <iostream>

// Update and Draw one frame
void UpdateDrawFrame(void* renderer) {
  static_cast<Renderer*>(renderer)->Draw();
}

int main() {
  SetConfigFlags(FLAG_VSYNC_HINT);
  InitWindow(metrics::kWindowWidth, metrics::kWindowHeight, "Head Shot");

  Timer GameTimer;
  float GameTime = 0;
  int GameFrame = 0;

  std::array<input::Input, 5400> GameInputs; // pour chaque joueur

  // network manager a une reference au renderer et au game

  // pres pk les var glob sont code smell

  Game game;

  Renderer renderer;

  renderer.Setup(&game);

  Audio audio{};
  audio.Setup();

#ifdef PLATFORM_WEB
  emscripten_set_main_loop_arg(UpdateDrawFrame, &renderer, 0, 1);

#else

  while (!WindowShouldClose()) {
    switch (game.GetState()) {
      case GameState::kNone:
        break;
      case GameState::kInGame:
        if (GameTime <= 0) {
          GameTimer.SetUp();
        }
        GameTimer.Tick();
        GameTime += GameTimer.DeltaTime;
        GameFrame++;

        input::Input frameInput = 0;

        if (IsKeyDown(KEY_D)) {
          frameInput |= input::kRight;
        }
        if (IsKeyDown(KEY_A)) {
          frameInput |= input::kLeft;
        }
        if (IsKeyDown(KEY_SPACE) || IsKeyPressed(KEY_W)) {
          frameInput |= input::kJump;
        }

        // game_data::GameInputs[game_data::GameFrame] = frameInput;
        game.SetInput(frameInput);

        game.Update(GameTimer.DeltaTime);

        renderer.SetGameTime(GameTime);
        break;
    }
    renderer.Draw();

#ifdef TRACY_ENABLE
    FrameMark;
#endif
  }

#endif

  renderer.TearDown();
  audio.TearDown();
  game.TearDown();

  return EXIT_SUCCESS;
}