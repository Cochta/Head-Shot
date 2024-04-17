#include "application.h"

// Update and Draw one frame
void UpdateDrawFrame(void* renderer) {
  static_cast<Renderer*>(renderer)->Draw();
}

void Application::Setup() {
  SetConfigFlags(FLAG_VSYNC_HINT);
  InitWindow(metrics::kWindowWidth, metrics::kWindowHeight, "Head Shot");

  renderer_.Setup(&game_, &network_);
  audio_.Setup();

  network_.Connect();
}

void Application::Run() {
#ifdef PLATFORM_WEB
  emscripten_set_main_loop_arg(UpdateDrawFrame, &renderer, 0, 1);

#else

  while (!WindowShouldClose()) {
    network_.Service();
    switch (game_.GetState()) {
      case GameState::kMenu:
        break;
      case GameState::kInGame: {
        if (game_time_ <= 0) {
          game_timer_.SetUp();
        }
        game_timer_.Tick();
        game_time_ += game_timer_.DeltaTime;
        game_frame_++;

        input::Input frameInput = 0;

        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
          frameInput |= input::kRight;
        }
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
          frameInput |= input::kLeft;
        }
        if (IsKeyDown(KEY_SPACE) || IsKeyPressed(KEY_W) || IsKeyDown(KEY_UP)) {
          frameInput |= input::kJump;
        }
        game_.SetInput(frameInput);
        ExitGames::Common::Hashtable data;
        data.put(static_cast<nByte>(PacketKey::kInput), frameInput);

        network_.RaiseEvent(true, PacketType::kInput, data);

        //const auto frameInutes = game_.GetLastFrameInputs();
        //ExitGames::Common::Hashtable inputData;
        //inputData.put(static_cast<nByte>(PacketKey::kLastFrameInputs),
        //              frameInutes);
        //network_.RaiseEvent(true, PacketType::kLastFrameInputs, inputData);

        game_.Update(game_timer_.DeltaTime);

        renderer_.SetGameTime(game_time_);
      } break;
      case GameState::kGameFinished:
        // network leave room
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
  network_.Disconnect();
  renderer_.TearDown();
  audio_.TearDown();
  game_.TearDown();
}
