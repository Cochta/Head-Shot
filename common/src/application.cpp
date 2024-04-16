#include "application.h"

// Update and Draw one frame
void UpdateDrawFrame(void* renderer) {
  static_cast<Renderer*>(renderer)->Draw();
}

void Application::Setup() {
  SetConfigFlags(FLAG_VSYNC_HINT);
  InitWindow(metrics::kWindowWidth, metrics::kWindowHeight, "Head Shot");

  network_.Connect();

  renderer_.Setup(&game_, &network_);
  audio_.Setup();
}

void Application::Run() {
#ifdef PLATFORM_WEB
  emscripten_set_main_loop_arg(UpdateDrawFrame, &renderer, 0, 1);

#else

  while (!WindowShouldClose()) {
    network_.Service();
    switch (game_.GetState()) {
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
        game_.SetInput(frameInput);
        ExitGames::Common::Hashtable data;
        data.put(static_cast<nByte>(PacketKey::kInput), frameInput);
        network_.RaiseEvent(true, PacketType::kInput, data);

        game_.Update(GameTimer.DeltaTime);

        renderer_.SetGameTime(GameTime);
        break;
    }
    renderer_.Draw();

#ifdef TRACY_ENABLE
    FrameMark;
#endif
  }
#endif
}

void Application::TearDown() {
  renderer_.TearDown();
  audio_.TearDown();
  game_.TearDown();
}
