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
  InitWindow(Metrics::Width, Metrics::Height, "Head Shot ");

  Metrics::BallType = Metrics::BallType::TENNISBALL;

  Renderer renderer;

  renderer.Setup();

  Audio audio;
  audio.Setup();

  Game game;

  PlaySound(audio.music);

#ifdef PLATFORM_WEB
  emscripten_set_main_loop_arg(UpdateDrawFrame, &renderer, 0, 1);

#else

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    switch (Metrics::State) {
      case Metrics::GameState::MENU:
        break;
      case Metrics::GameState::STARTGAME:
        game.Setup();
        Metrics::State = Metrics::GameState::INGAME;
        break;
      case Metrics::GameState::INGAME:
        if (IsKeyDown(KEY_D)) {
          game.ProcessInputP1(Input::Right);
        }
        if (IsKeyDown(KEY_A)) {
          game.ProcessInputP1(Input::Left);
        }
        if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W)) {
          game.ProcessInputP1(Input::Jump);
        }
        if (IsKeyDown(KEY_RIGHT)) {
          game.ProcessInputP2(Input::Right);
        }
        if (IsKeyDown(KEY_LEFT)) {
          game.ProcessInputP2(Input::Left);
        }
        if (IsKeyPressed(KEY_UP)) {
          game.ProcessInputP2(Input::Jump);
        }
        game.Update();
        break;
    }
    renderer.Draw();
  }

#endif

  renderer.TearDown();
  audio.TearDown();
  game.TearDown();

  return EXIT_SUCCESS;
}