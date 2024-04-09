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
  InitWindow(metrics::Width, metrics::Height, "Head Shot ");

  game_data::Type = game_data::BallType::BASKETBALL;

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
    switch (game_data::State) {
      case game_data::GameState::MENU:
        break;
      case game_data::GameState::STARTGAME:
        game.Setup();
        game_data::State = game_data::GameState::INGAME;

        game_data::GameTimer.SetUp();
        break;
      case game_data::GameState::INGAME:
        game_data::GameTimer.Tick();
        game_data::GameTime += game_data::GameTimer.DeltaTime;
        game_data::GameFrame++;

        input::Input frameInput = 0;

        if (IsKeyDown(KEY_D)) {
          frameInput |= input::kRight;
          game.ProcessInputP1(Input::Right);
        }
        if (IsKeyDown(KEY_A)) {
          frameInput |= input::kLeft;
          game.ProcessInputP1(Input::Left);
        }
        if (IsKeyDown(KEY_SPACE) || IsKeyPressed(KEY_W)) {
          frameInput |= input::kJump;
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
        game_data::GameInputs[game_data::GameFrame] = frameInput;
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