#pragma once

#include <iostream>

#include "Game.h"
#include "metrics.h"

class Rollback {
 public:
  void RegisterGame(Game* game) noexcept {
    current_ = game;
    confirmed_.SetBallType(game->GetBallType());
    confirmed_.StartGame();
    confirmed_.player_nbr = current_->player_nbr;
  }

  void SetPlayerInput(const input::FrameInput &frame_input, int player_id);
  void SetOtherPlayerInput(const std::vector<input::FrameInput>& frame_inputs,
                           int player_id);

  void DoRollback() const noexcept;

  int ConfirmFrame() noexcept;

  const input::Input& GetLastPlayerInput(const int player_id) const noexcept;

  [[nodiscard]] short GetConfirmedFrame() const noexcept {
    return confirmed_frame_;
  }
  [[nodiscard]] short GetCurentFrame() const noexcept {
    return current_frame_;
  }

  [[nodiscard]] short GetLastRemoteInputFrame() const noexcept {
    return last_remote_input_frame_;
  }

  [[nodiscard]] short GetFrameToConfirm() const noexcept {
    return frame_to_confirm_;
  }

  void IncreaseCurrentFrame() noexcept { current_frame_++; }

 private:
  Game* current_ = nullptr;
  Game confirmed_{this};

  short current_frame_ = -1;
  short last_remote_input_frame_ = -1;
  short frame_to_confirm_ = 0;
  short confirmed_frame_ = -1;

  std::array<input::Input, 2> last_inputs_;

  std::array<std::array<input::Input, metrics::kGameFrameNbr>, 2> inputs_{};
};