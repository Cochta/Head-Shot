#include "rollback.h"

 void Rollback::SetLocalPlayerInput(input::FrameInput frame_input,
                                           int player_id) {
   inputs_[player_id][frame_input.frame_nbr] = frame_input.input;
   current_frame_ = frame_input.frame_nbr;
 }

void Rollback::SetRemotePlayerInput(
    const std::vector<input::FrameInput>& frame_inputs, int player_id) {
  auto last_remote_frame_input = frame_inputs.back();
  const auto frame_diff =
      last_remote_frame_input.frame_nbr - last_remote_input_frame_;

  // If the last remote input is greater than the local current frame, set the
  // last_remote_frame_input value with the remote input value for my
  // current_frame to avoid the confirmation of a future frame without the local
  // inputs.
  if (last_remote_frame_input.frame_nbr > current_frame_) {
    const auto& current_frame_it =
        std::find_if(frame_inputs.begin(), frame_inputs.end(),
                     [this](input::FrameInput frame_input) {
                       return frame_input.frame_nbr == current_frame_;
                     });

    last_remote_frame_input = *current_frame_it;
  }

  // The inputs are already received.
  if (frame_diff < 1) {
    return;
  }

  // If we didn't receive some inputs between the last time and the new inputs,
  // iterates over the missing inputs to add them in the inputs array.
  const auto it = std::find_if(frame_inputs.begin(), frame_inputs.end(),
                               [this](input::FrameInput frame_input) {
                                 return frame_input.frame_nbr ==
                                        last_remote_input_frame_ + 1;
                               });

  auto idx = std::distance(frame_inputs.begin(), it);
  for (int i = last_remote_input_frame_ + 1;
       i < last_remote_frame_input.frame_nbr; i++) {
    const auto input = frame_inputs[idx].input;

    inputs_[player_id][i] = input;

    idx++;
  }

  // Predicts the remote client would not change its inputs until the current
  // frame to have a realtime simulation.
  for (short frame = last_remote_frame_input.frame_nbr; frame <= current_frame_;
       frame++) {
    inputs_[player_id][frame] = last_remote_frame_input.input;
  }

  SimulateUntilCurrentFrame();

  last_remote_input_frame_ = last_remote_frame_input.frame_nbr;
}

void Rollback::SimulateUntilCurrentFrame() noexcept {
  *current_game_ = confirmed_game_;

  for (short frame = static_cast<short>(confirmed_frame_ + 1);
       frame < current_frame_; frame++) {

    current_game_->FixedUpdate(frame);
  }

  // The Fixed update of the current frame is made in the main loop after
  // polling received events from network.
}

int Rollback::ComputeFrameToConfirmChecksum() noexcept {
  game_to_confirm_ = confirmed_game_;

  for (int frame = confirmed_frame_ + 1; frame <= frame_to_confirm_; frame++) {

    game_to_confirm_.FixedUpdate(frame);
  }

  return game_to_confirm_.CheckSum();
}

void Rollback::ConfirmFrame() noexcept {
  for (int frame = confirmed_frame_ + 1; frame <= frame_to_confirm_; frame++) {

    confirmed_game_.FixedUpdate(frame);
  }

  confirmed_frame_++;
  frame_to_confirm_++;
}

input::Input Rollback::GetPlayerInputAtFrame(
    int player_id, short frame_nbr) const noexcept {
  return inputs_[player_id][frame_nbr];
}