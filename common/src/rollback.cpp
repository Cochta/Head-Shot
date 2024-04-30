#include "rollback.h"

void Rollback::SetPlayerInput(const input::FrameInput& local_input,
                              int player_id) {
  inputs_[player_id][local_input.frame_nbr] = local_input.input;
  last_inputs_[player_id] = local_input.input;
}

void Rollback::SetOtherPlayerInput(
    const std::vector<input::FrameInput>& new_remote_inputs, int player_id) {
  // Retrieve the last remote frame input
  auto last_new_remote_input = new_remote_inputs.back();

  // Calculate the difference between the last new remote frame and the last
  // remote input frame
  const auto frame_diff =
      last_new_remote_input.frame_nbr - last_remote_input_frame_;

  // If no new inputs received, return
  if (frame_diff < 1) {
    return;
  }

  // If the last remote input frame is greater than the current frame, adjust
  // last_new_remote_input
  if (last_new_remote_input.frame_nbr > current_frame_) {
    const auto& current_frame_it =
        std::find_if(new_remote_inputs.begin(), new_remote_inputs.end(),
                     [this](const input::FrameInput& frame_input) {
                       return frame_input.frame_nbr == current_frame_;
                     });
    last_new_remote_input = *current_frame_it;
  }

  // Find the position of the first missing input
  auto missing_input_it = std::find_if(
      new_remote_inputs.begin(), new_remote_inputs.end(),
      [this](const input::FrameInput& frame_input) {
        return frame_input.frame_nbr == last_remote_input_frame_ + 1;
      });

  // Check if rollback is necessary
  bool must_rollback = last_remote_input_frame_ == -1;

  // Iterate over the missing inputs and update the inputs array
  for (short frame = last_remote_input_frame_ + 1;
       frame <= last_new_remote_input.frame_nbr; frame++) {
    // Get the input for the current frame
    const auto input = missing_input_it->input;

    // Check if rollback is necessary
    if (last_remote_input_frame_ > -1 && input != last_inputs_[player_id]) {
      must_rollback = true;
    }

    // Update the inputs array
    inputs_[player_id][frame] = missing_input_it->input;

    // Move to the next missing input
    ++missing_input_it;
  }

  // Predict inputs for frames up to the current frame with the last remote
  // input.
  for (short frame = last_new_remote_input.frame_nbr; frame <= current_frame_;
       frame++) {
    inputs_[player_id][frame] = last_new_remote_input.input;
  }

  if (must_rollback) {
    DoRollback();
  }

  // Update last inputs and last remote input frame.
  last_inputs_[player_id] = last_new_remote_input.input;
  last_remote_input_frame_ = last_new_remote_input.frame_nbr;
}

void Rollback::DoRollback() const noexcept {
#ifdef TRACY_ENABLE
  ZoneScoped;
#endif

  current_->Copy(confirmed_);

  for (short frame = static_cast<short>(confirmed_frame_ + 1);
       frame < current_frame_; frame++) {
    for (int player_id = 0; player_id < 2; player_id++) {
      const auto input = inputs_[player_id][frame];
      if (current_->player_nbr == player_id) {
        current_->SetPlayerInput(input);
      } else {
        current_->SetOtherPlayerInput(input);
      }
    }

    current_->FixedUpdate();
  }

  // The Fixed update of the current frame is made in the main loop after
  // polling received events from network.
}

int Rollback::ConfirmFrame() noexcept {
  for (int player_id = 0; player_id < 2; player_id++) {
    const auto input = inputs_[player_id][frame_to_confirm_];
    if (confirmed_.player_nbr == player_id) {
      confirmed_.SetPlayerInput(input);
    } else {
      confirmed_.SetOtherPlayerInput(input);
    }
  }

  confirmed_.FixedUpdate();
  const auto checksum = confirmed_.CheckSum();

  confirmed_frame_++;
  frame_to_confirm_++;
  return checksum;
}

const input::Input& Rollback::GetLastPlayerInput(
    const int player_id) const noexcept {
  return last_inputs_[player_id];
}
