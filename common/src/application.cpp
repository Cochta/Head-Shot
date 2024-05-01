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
  float time = metrics::kFixedDeltaTime;

  while (!WindowShouldClose()) {
    audio_.Update();
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

        time += game_timer_.DeltaTime;
        while (time >= metrics::kFixedDeltaTime) {
          rollback_.IncreaseCurrentFrame();
          if (rollback_.GetCurentFrame() >= metrics::kGameFrameNbr) {
            game_.EndGame();
            network_.LeaveRoom();
            break;
          }

          HandlePacket();

          input::Input actualInput = 0;

          if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
            actualInput |= input::kRight;
          }
          if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
            actualInput |= input::kLeft;
          }
          if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
            actualInput |= input::kJump;
          }
          if (IsKeyDown(KEY_SPACE)) {
            actualInput |= input::kKick;
          }

          const input::FrameInput frame_input{actualInput,
                                              rollback_.GetCurentFrame()};
          rollback_.SetPlayerInput(frame_input, game_.player_nbr);

          inputs_.push_back(actualInput);
          frames_.push_back(rollback_.GetCurentFrame());

          ExitGames::Common::Hashtable event_data;
          event_data.put(static_cast<nByte>(PacketKey::kInput), inputs_.data(),
                         static_cast<int>(inputs_.size()));
          event_data.put(static_cast<nByte>(PacketKey::kFrame), frames_.data(),
                         static_cast<int>(frames_.size()));

          network_.RaiseEvent(false, PacketType::kInput, event_data);

          for (size_t i = 0; i < 2; i++) {
            const auto input = rollback_.GetLastPlayerInput(i);
            if (i == game_.player_nbr) {
              game_.SetPlayerInput(input);
            } else {
              game_.SetOtherPlayerInput(input);
            }
          }

          game_.Update();

          time -= metrics::kFixedDeltaTime;
        }

        renderer_.SetGameTime(game_time_);
      } break;
      case GameState::kGameFinished:
        time = metrics::kFixedDeltaTime;
        rollback_.Reset();
        while (!packet_queue.empty()) {
          packet_queue.pop();
        }
        game_time_ = 0;
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

void Application::HandlePacket() {
  while (!packet_queue.empty()) {
    const auto& packet = packet_queue.front();

    switch (packet.type) {
      case PacketType::kInput: {
        std::vector<input::FrameInput> frameInputs;
        const auto input_value =
            packet.data.getValue(static_cast<nByte>(PacketKey::kInput));

        const input::Input* inputs =
            ExitGames::Common::ValueObject<input::Input*>(input_value)
                .getDataCopy();

        const int inputs_count =
            *ExitGames::Common::ValueObject<input::Input*>(input_value)
                 .getSizes();

        const auto frame_value =
            packet.data.getValue(static_cast<nByte>(PacketKey::kFrame));

        const short* frames =
            ExitGames::Common::ValueObject<short*>(frame_value).getDataCopy();

        for (int i = 0; i < inputs_count; i++) {
          input::FrameInput frame_input{inputs[i], frames[i]};
          frameInputs.push_back(frame_input);
        }

        if (frameInputs.back().frame_nbr <
            rollback_.GetLastRemoteInputFrame()) {
          // received old input, no need to send confirm packet.
          return;
        }

        rollback_.SetOtherPlayerInput(frameInputs,
                                      game_.player_nbr == 0 ? 1 : 0);

        if (game_.player_nbr == 0) {
          SendFrameConfirmation(frameInputs);
        }

        ExitGames::Common::MemoryManagement::deallocateArray(inputs);
        ExitGames::Common::MemoryManagement::deallocateArray(frames);

      } break;
      case PacketType::kFrameConfirmation: {
        if (game_.player_nbr == 0) {
          inputs_.erase(inputs_.begin());
          frames_.erase(frames_.begin());
          break;
        }

        int checksum = 0;
        std::vector<input::FrameInput> frame_inputs{};

        const auto checksum_value =
            packet.data.getValue(static_cast<nByte>(PacketKey::kChecksum));
        checksum =
            ExitGames::Common::ValueObject<int>(checksum_value).getDataCopy();

        const auto input_value =
            packet.data.getValue(static_cast<nByte>(PacketKey::kInput));

        const input::Input* inputs =
            ExitGames::Common::ValueObject<input::Input*>(input_value)
                .getDataCopy();

        const int inputs_count =
            *ExitGames::Common::ValueObject<input::Input*>(input_value)
                 .getSizes();

        const auto frame_value =
            packet.data.getValue(static_cast<nByte>(PacketKey::kFrame));

        const short* frames =
            ExitGames::Common::ValueObject<short*>(frame_value).getDataCopy();

        for (int i = 0; i < inputs_count; i++) {
          input::FrameInput frame_input{inputs[i], frames[i]};
          frame_inputs.push_back(frame_input);
        }

        // If we did not receive the inputs before the frame to confirm, add
        // them.
        if (rollback_.GetLastRemoteInputFrame() <
            frame_inputs.back().frame_nbr) {
          const int other_client_id = game_.player_nbr == 0 ? 1 : 0;
          rollback_.SetOtherPlayerInput(frame_inputs, other_client_id);
        }

        const int check_sum = rollback_.ConfirmFrame();

        if (check_sum != checksum) {
          std::cerr << "Not same checksum for frame: "
                    << rollback_.GetFrameToConfirm() << '\n';
          return;
        }

        // Send a frame confirmation event with empty data to the master client
        // just to tell him that we confirmed the frame and that he can erase
        // the input at the confirmed frame in its vector of inputs.
        network_.RaiseEvent(true, PacketType::kFrameConfirmation,
                            ExitGames::Common::Hashtable());

        inputs_.erase(inputs_.begin());
        frames_.erase(frames_.begin());
        ExitGames::Common::MemoryManagement::deallocateArray(inputs);
        ExitGames::Common::MemoryManagement::deallocateArray(frames);
      } break;
    }
    packet_queue.pop();
  }
}

void Application::SendFrameConfirmation(
    const std::vector<input::FrameInput>& remote_frame_inputs) noexcept {
  auto frame_to_confirm_it = std::find_if(
      remote_frame_inputs.begin(), remote_frame_inputs.end(),
      [this](const input::FrameInput& frame_input) {
        return frame_input.frame_nbr == rollback_.GetFrameToConfirm();
      });

  auto end_it = remote_frame_inputs.end();

  // If the last remote inputs is greater than the current frame. The end
  // iterator must be equal to the frame input of the local current frame.
  if (remote_frame_inputs.back().frame_nbr > rollback_.GetCurentFrame()) {
    // Get the iterator of the inputs at the current frame to avoid to confirm
    // a frame greater than the local current frame.
    const auto current_frame_it = std::find_if(
        remote_frame_inputs.begin(), remote_frame_inputs.end(),
        [this](const input::FrameInput& frame_input) {
          return frame_input.frame_nbr == rollback_.GetCurentFrame();
        });

    end_it = current_frame_it;
  }

  while (frame_to_confirm_it != end_it) {
    const auto frame_to_confirm = *frame_to_confirm_it;

    if (frame_to_confirm.frame_nbr > rollback_.GetCurentFrame()) {
      // Tried to confirm a frame greater than the local current frame.
      break;
    }

    const int check_sum = rollback_.ConfirmFrame();

    ExitGames::Common::Hashtable event_check_sum;
    event_check_sum.put(static_cast<nByte>(PacketKey::kChecksum), check_sum);
    event_check_sum.put(static_cast<nByte>(PacketKey::kInput), inputs_.data(),
                        inputs_.size());
    event_check_sum.put(static_cast<nByte>(PacketKey::kFrame), frames_.data(),
                        frames_.size());

    network_.RaiseEvent(true, PacketType::kFrameConfirmation, event_check_sum);

    ++frame_to_confirm_it;
  }
}
