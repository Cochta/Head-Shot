#pragma once

#include <iostream>

#include "Game.h"
#include "metrics.h"

/**
 * \brief Rollback is a class responsible of the integrity of the game
 * simulation.
 *
 * It has 3 game states with different temporalities.
 *
 * The current game state, which is the local client's real-time game.
 *
 * The confirmed state, which is the last game state confirmed by the client.
 * (A checksum is done to confirm the integrity of the simulation.)
 *
 * Finally, the state to be confirmed is the game state calculated by the
 * master client once all the inputs for a frame have been received.
 * All other clients receive the checksum of this game state and verify if it
 * corresponds to their checksum for this given state.
 */
class Rollback {
 public:
  void RegisterGame(Game* game) noexcept {
    current_game_ = game;
    confirmed_game_ = *game;

    game_to_confirm_ = *game;
  }

  void SetLocalPlayerInput(input::FrameInput frame_input, int player_id);
  void SetRemotePlayerInput(const std::vector<input::FrameInput>& frame_inputs,
                            int player_id);

  void SimulateUntilCurrentFrame() noexcept;
  [[nodiscard]] int ComputeFrameToConfirmChecksum() noexcept;
  void ConfirmFrame() noexcept;

  [[nodiscard]] input::Input GetPlayerInputAtFrame(
      int player_id, short frame_nbr) const noexcept;

  [[nodiscard]] short confirmed_frame() const noexcept {
    return confirmed_frame_;
  }

  [[nodiscard]] short last_remote_input_frame() const noexcept {
    return last_remote_input_frame_;
  }

  [[nodiscard]] short frame_to_confirm() const noexcept {
    return frame_to_confirm_;
  }

 private:
  /**
   * \brief current_game_ is a pointer to local client's Game.
   */
  Game* current_game_ = nullptr;

  /**
   * \brief confirmed_player_manager_ is a copy of the client's player_manager
   * at the last confirmed frame state.
   */
  Game confirmed_game_{this};

  /**
   * \brief game_to_confirm_ is a Game at the state that needs
   * to be confirmed.
   */
  Game game_to_confirm_{this};

  /**
   * \brief The frame nbr of the local client.
   */
  short current_frame_ = -1;

  /**
   * \brief The frame number of the last time a remote input was received.
   */
  short last_remote_input_frame_ = -1;

  /**
   * \brief The frame number which the master client wants to confirm.
   */
  short frame_to_confirm_ = 0;

  /**
   * \brief The frame number of the last confirmed frame (frame verified with
   * checksum).
   */
  short confirmed_frame_ = -1;

  /**
   * \brief kMaxFrameCount is the maximum of frame that the game can last.
   * Here 30'000 corresponds to 10 minutes.
   */
  static constexpr short kMaxFrameCount = 5400;

  std::array<std::array<input::Input, kMaxFrameCount>, 2> inputs_{};
};