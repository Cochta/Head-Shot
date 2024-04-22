#include "application.h"

// Update and Draw one frame
void UpdateDrawFrame(void* renderer) {
  static_cast<Renderer*>(renderer)->Draw();
}

void Application::Setup() {
  SetConfigFlags(FLAG_VSYNC_HINT);
  InitWindow(metrics::kWindowWidth, metrics::kWindowHeight, "Head Shot");
  renderer_.Setup(&game_, &network_);
  // audio_.Setup();

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

        // game_.SetInput(input);
        // ExitGames::Common::Hashtable data;
        // data.put(static_cast<nByte>(PacketKey::kInput), input);

        // network_.RaiseEvent(true, PacketType::kInput, data);

        static float time = metrics::kFixedDeltaTime;
        time += game_timer_.DeltaTime;
        while (time >= metrics::kFixedDeltaTime) {
          game_frame_++;
          input::Input input = 0;

          if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
            input |= input::kRight;
          }
          if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
            input |= input::kLeft;
          }
          if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
            input |= input::kJump;
          }
          if (IsKeyDown(KEY_SPACE)) {
            input |= input::kKick;
          }

          //------------------------------------------
          const input::FrameInput frame_input{input, game_frame_};
          rollback_.SetLocalPlayerInput(frame_input, game_.player_nbr);

          inputs_.push_back(input);
          frames_.push_back(game_frame_);

          ExitGames::Common::Hashtable event_data;
          event_data.put(static_cast<nByte>(PacketKey::kInput), inputs_.data(),
                         static_cast<int>(inputs_.size()));
          event_data.put(static_cast<nByte>(PacketKey::kFrame), frames_.data(),
                         static_cast<int>(frames_.size()));

          network_.RaiseEvent(false, PacketType::kInput, event_data);

          if (inputs_.size() >= 150) {
            inputs_.erase(inputs_.begin());
            frames_.erase(frames_.begin());
          }
          //------------------------------------------

          game_.Update(game_timer_.DeltaTime);

          time -= metrics::kFixedDeltaTime;
        }

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
