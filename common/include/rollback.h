#pragma once

#include <iostream>

#include "Game.h"
#include "metrics.h"

class Rollback {
 public:
  void RegisterGame(Game* game) noexcept {
    current_ = game;
    confirmed_ = *game;

    to_confirm_ = *game;
  }

  void SetPlayerInput(input::FrameInput frame_input, int player_id);
  void SetOtherPlayerInput(const std::vector<input::FrameInput>& frame_inputs,
                            int player_id);

  void DoRollback() noexcept;

  [[nodiscard]] int ConfirmChecksum() noexcept;
  void ConfirmFrame() noexcept;

  [[nodiscard]] input::Input GetPlayerInputAtFrame(
      int player_id, short frame_nbr) const noexcept;

  [[nodiscard]] short GetConfirmedFrame() const noexcept {
    return confirmed_frame_;
  }

  [[nodiscard]] short GetLastRemoteInputFrame() const noexcept {
    return last_remote_input_frame_;
  }

  [[nodiscard]] short GetFrameToConfirm() const noexcept {
    return frame_to_confirm_;
  }

 private:
  Game* current_ = nullptr;
  Game confirmed_{this};
  Game to_confirm_{this};

  short current_frame_ = -1;
  short last_remote_input_frame_ = -1;
  short frame_to_confirm_ = 0;
  short confirmed_frame_ = -1;
  static constexpr short kMaxFrameCount = 5400; // 1:30 min at 60 hertz

  std::array<std::array<input::Input, kMaxFrameCount>, 2> inputs_{};
};