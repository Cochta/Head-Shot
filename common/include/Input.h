#pragma once

#include <cstdint>

namespace input {

/**
 * @typedef Input
 * @brief Alias for the standard integer type `std::uint8_t` used to represent
 * input actions.
 */
using Input = std::uint8_t;

/**
 * @brief Bitmask representing the "jump" action.
 *
 * This constant represents the action of jumping in the game. It is defined using
 * a bitmask, allowing it to be combined with other input actions in an Input
 * variable using bitwise OR operations.
 */
constexpr Input kJump = 1 << 0;

/**
 * @brief Bitmask representing the "left" action.
 *
 * This constant represents the action of moving left in the game. It is defined
 * using a bitmask, allowing it to be combined with other input actions in an
 * Input variable using bitwise OR operations.
 */
constexpr Input kLeft = 1 << 1;

/**
 * @brief Bitmask representing the "right" action.
 *
 * This constant represents the action of moving right in the game. It is defined
 * using a bitmask, allowing it to be combined with other input actions in an
 * Input variable using bitwise OR operations.
 */
constexpr Input kRight = 1 << 2;

/**
 * @brief Bitmask representing the "kick" action.
 *
 * This constant represents the action of kicking in the game. It is defined using
 * a bitmask, allowing it to be combined with other input actions in an Input
 * variable using bitwise OR operations.
 */
constexpr Input kKick = 1 << 3;

/**
 * @struct FrameInput
 * @brief Represents the input data for a single frame.
 *
 * This struct holds the input actions and the frame number for a single frame
 * of a game.
 *
 * @var input The input actions for the frame, represented as an Input type
 * (std::uint8_t). It uses bitwise operations to combine different actions
 * (e.g., kJump, kLeft, kRight, kKick). Default value is 0, indicating no
 * action.
 *
 * @var frame_nbr The frame number for the input data. It represents the current
 * frame in the game. Default value is 0.
 */
struct FrameInput {
  Input input = 0;      ///< The input actions for the frame.
  short frame_nbr = 0;  ///< The frame number for the input data.
};

}  // namespace input