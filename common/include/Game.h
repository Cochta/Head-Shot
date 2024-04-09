#pragma once
#include "GameData.h"

#include "Metrics.h"
#include "Timer.h"
#include "World.h"

enum class Input {
  Right,
  Left,
  Jump

};

class Game : public ContactListener {
 public:
 private:
  World _world;
  Timer _timer;

  std::vector<BodyRef> _body_refs;
  std::vector<ColliderRef> _col_refs;

  ColliderRef _ground_col_ref;

  ColliderRef _player_blue_col_ref;
  // BodyRef _player_blue_feet_body_ref;
  // ColliderRef _player_blue_feet_col_ref;

  ColliderRef _player_red_col_ref;

  BodyRef _ball_body_ref;

  BodyRef _player_blue_body_ref;
  BodyRef _player_red_body_ref;

  bool _is_player_blue_grounded = false;
  bool _is_player_red_grounded = false;

  static constexpr float kBallGravity = 1000;
  static constexpr float kPlayerGravity = 2500;
  static constexpr float kWalkSpeed = 1500;
  static constexpr float kMaxSpeed = 1200;
  static constexpr float kJumpSpeed = -50000;
  static constexpr float kPlayerMass = 5;

 public:
  void ProcessInputP1(Input input) noexcept;
  void ProcessInputP2(Input input) noexcept;

  void Setup() noexcept;
  void Update() noexcept;
  void TearDown() noexcept;

  void OnTriggerEnter(ColliderRef col1, ColliderRef col2) noexcept override{};

  void OnTriggerExit(ColliderRef col1, ColliderRef col2) noexcept override{};

  void OnCollisionEnter(ColliderRef col1, ColliderRef col2) noexcept override;

  void OnCollisionExit(ColliderRef col1, ColliderRef col2) noexcept override{};

 private:
  void CreateBall() noexcept;
  void CreateTerrain() noexcept;
  void CreatePlayers() noexcept;
};