#pragma once
#include "Input.h"
#include "Metrics.h"
#include "World.h"

class Rollback;

enum class BallType {
  kFootball = 0,
  kBasketball,
  kBaseball,
  kVolleyball,
  kTennisball,
  kCount
};

enum class GameState { kMenu, kInGame, kGameFinished };

/**
 * \brief Handles the physics state of the app
 */
class Game : public ContactListener {
 private:
  World world_;

  Rollback* rollback_;

  GameState state_ = GameState::kMenu;

  input::Input input_{};
  input::Input other_player_input_{};

  std::vector<BodyRef> body_refs_;
  std::vector<ColliderRef> col_refs_;

  ColliderRef ground_col_ref_{};

  ColliderRef player_blue_col_ref_{};

  ColliderRef player_blue_feet_col_ref_;

  float player_blue_shoot_time_ = 1.f;

  bool can_player_blue_shoot_ = false;

  ColliderRef player_red_col_ref_{};

  ColliderRef player_red_feet_col_ref_;

  float player_red_shoot_time_ = 1.f;

  bool can_player_red_shoot_ = false;

  BallType ball_type_ = BallType::kFootball;
  BodyRef ball_body_ref_{};
  ColliderRef ball_col_ref_{};
  float ball_radius_ = metrics::kBallRadiusMedium;

  int blue_score_ = 0;
  int red_score_ = 0;

  ColliderRef left_goal_col_ref_{};
  ColliderRef right_goal_col_ref_{};

  BodyRef player_blue_body_ref_{};
  BodyRef player_red_body_ref_{};

  bool is_player_blue_grounded_ = false;
  bool is_player_red_grounded_ = false;

  static constexpr float kBallGravity = 1000;
  static constexpr float kPlayerGravity = 2500;
  static constexpr float kWalkSpeed = 1500;
  static constexpr float kMaxSpeed = 1200;
  static constexpr float kJumpSpeed = -75000;
  static constexpr float kPlayerMass = 5;

  static constexpr float kShootForce = 25000;

 public:
  int player_nbr = -1;
  Game(Rollback* rollback);

  void Copy(const Game& other);

  void ProcessInput() noexcept;

  void Setup() noexcept;
  void Update() noexcept;
  void FixedUpdate();
  void TearDown() noexcept;

  void StartGame();
  GameState GetState();

  float GetBallRadius() const noexcept;
  Math::Vec2F GetBallPosition() noexcept;
  Math::Vec2F GetBallVelocity() noexcept;
  BallType GetBallType() noexcept;

  int GetBlueScore() noexcept { return blue_score_; }
  int GetRedScore() noexcept { return red_score_; }

  Math::Vec2F GetPlayerBluePos() noexcept;
  Math::Vec2F GetPlayerRedPos() noexcept;

  void SetPlayerInput(input::Input input) noexcept;
  void SetOtherPlayerInput(input::Input input) noexcept;

  void SetBallType(BallType type) noexcept { ball_type_ = type; }

  void EndGame();

  void OnTriggerEnter(ColliderRef col1, ColliderRef col2) noexcept override;

  void OnTriggerExit(ColliderRef col1, ColliderRef col2) noexcept override;

  void OnCollisionEnter(ColliderRef col1, ColliderRef col2) noexcept override;

  void OnCollisionExit(ColliderRef col1, ColliderRef col2) noexcept override{};

  int CheckSum() noexcept;

  void ResetPositions() noexcept;

 private:
  void CreateBall() noexcept;
  void CreateTerrain() noexcept;
  void CreatePlayers() noexcept;
};