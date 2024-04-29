#pragma once

#include "Audio.h"
#include "Renderer.h"
#include "network.h"
#include "rollback.h"

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#else
#endif
#include <queue>

class Application {
 public:
  std::queue<Packet> packet_queue{};

  void Setup();
  void Run();
  void TearDown();

 private:
  Rollback rollback_;
  Game game_{&rollback_};

  Audio audio_{};
  Renderer renderer_{};
  Network network_{PHOTON_APP_ID, "V1.0", &game_, &renderer_, &rollback_, this};

  Timer game_timer_;
  float game_time_ = 0;

  std::vector<input::Input> inputs_;
  std::vector<short> frames_;

 private:
  void HandlePacket();
  void SendFrameConfirmation(
      const std::vector<input::FrameInput>& remote_frame_inputs) noexcept;
};