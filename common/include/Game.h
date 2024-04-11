#pragma once
#include "Input.h"
#include "Metrics.h"
#include "Timer.h"
#include "World.h"

enum class BallType {
  kFootball = 0,
  kVolleyball,
  kBasketball,
  kTennisball,
  kBaseball,
  kCount
};

enum class GameState { kNone, kInGame };

/**
 * \brief Handles the physics state of the app
 */
class Game : public ContactListener {
 private:
  World world_;
  Timer timer_;

  GameState state_ = GameState::kNone;

  input::Input input_{};

  std::vector<BodyRef> body_refs_;
  std::vector<ColliderRef> col_refs_;

  ColliderRef ground_col_ref_{};

  ColliderRef player_blue_col_ref_{};
  // BodyRef _player_blue_feet_body_ref;
  // ColliderRef _player_blue_feet_col_ref;

  ColliderRef player_red_col_ref_{};

  BallType ball_type_ = BallType::kFootball;
  BodyRef ball_body_ref_{};
  float ball_radius_ = metrics::kBallRadiusMedium;

  BodyRef player_blue_body_ref_{};
  BodyRef player_red_body_ref_{};

  bool is_player_blue_grounded_ = false;
  bool is_player_red_grounded_ = false;

  static constexpr float kBallGravity = 1000;
  static constexpr float kPlayerGravity = 2500;
  static constexpr float kWalkSpeed = 1500;
  static constexpr float kMaxSpeed = 1200;
  static constexpr float kJumpSpeed = -50000;
  static constexpr float kPlayerMass = 5;

 public:
  void ProcessInputP1() noexcept;
  void ProcessInputP2() noexcept;

  void Setup() noexcept;
  void Update() noexcept;
  void TearDown() noexcept;

  void StartGame();
  GameState GetState();

  void SetInput(input::Input input);

  float GetBallRadius() const noexcept;
  Math::Vec2F GetBallPosition() noexcept;
  Math::Vec2F GetBallVelocity() noexcept;
  BallType GetBallType() noexcept;

  Math::Vec2F GetPlayerBluePos() noexcept;
  Math::Vec2F GetPlayerRedPos() noexcept;

  void OnTriggerEnter(ColliderRef col1, ColliderRef col2) noexcept override{};

  void OnTriggerExit(ColliderRef col1, ColliderRef col2) noexcept override{};

  void OnCollisionEnter(ColliderRef col1, ColliderRef col2) noexcept override;

  void OnCollisionExit(ColliderRef col1, ColliderRef col2) noexcept override{};

 private:
  void CreateBall() noexcept;
  void CreateTerrain() noexcept;
  void CreatePlayers() noexcept;
};