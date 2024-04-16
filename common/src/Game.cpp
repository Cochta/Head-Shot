#include "Game.h"

#ifdef TRACY_ENABLE
#include <TracyC.h>

#include <Tracy.hpp>
#endif

void Game::ProcessInput() noexcept {
  if (player_nbr == 1) {
    if (input_ & input::kRight) {
      world_.GetBody(player_blue_body_ref_).ApplyForce({kWalkSpeed, 0});
    }
    if (input_ & input::kLeft) {
      world_.GetBody(player_blue_body_ref_).ApplyForce({-kWalkSpeed, 0});
    }
    if ((input_ & input::kJump) && is_player_blue_grounded_) {
      world_.GetBody(player_blue_body_ref_).ApplyForce({0, kJumpSpeed});
      is_player_blue_grounded_ = false;
    }
    if (other_player_input_ & input::kRight) {
      world_.GetBody(player_red_body_ref_).ApplyForce({kWalkSpeed, 0});
    }
    if (other_player_input_ & input::kLeft) {
      world_.GetBody(player_red_body_ref_).ApplyForce({-kWalkSpeed, 0});
    }
    if ((other_player_input_ & input::kJump) && is_player_red_grounded_) {
      world_.GetBody(player_red_body_ref_).ApplyForce({0, kJumpSpeed});
      is_player_red_grounded_ = false;
    }
  } else {
    if (input_ & input::kRight) {
      world_.GetBody(player_red_body_ref_).ApplyForce({kWalkSpeed, 0});
    }
    if (input_ & input::kLeft) {
      world_.GetBody(player_red_body_ref_).ApplyForce({-kWalkSpeed, 0});
    }
    if ((input_ & input::kJump) && is_player_red_grounded_) {
      world_.GetBody(player_red_body_ref_).ApplyForce({0, kJumpSpeed});
      is_player_red_grounded_ = false;
    }
    if (other_player_input_ & input::kRight) {
      world_.GetBody(player_blue_body_ref_).ApplyForce({kWalkSpeed, 0});
    }
    if (other_player_input_ & input::kLeft) {
      world_.GetBody(player_blue_body_ref_).ApplyForce({-kWalkSpeed, 0});
    }
    if ((other_player_input_ & input::kJump) && is_player_blue_grounded_) {
      world_.GetBody(player_blue_body_ref_).ApplyForce({0, kJumpSpeed});
      is_player_blue_grounded_ = false;
    }
  }
}

void Game::Setup() noexcept {
  timer_.SetUp();
  world_.SetUp();
  world_.SetContactListener(this);
  CreateBall();
  CreateTerrain();
  CreatePlayers();
}

void Game::Update(float deltaTime) noexcept {
#ifdef TRACY_ENABLE
  ZoneScoped;
#endif
  static float time = kFixedDeltaTime;
  time += deltaTime;

  while (time >= kFixedDeltaTime) {
    switch (state_) {
      case GameState::kNone:
        break;
      case GameState::kInGame:
        ProcessInput();
        for (std::size_t i = 0; i < col_refs_.size(); ++i) {
          const auto& col = world_.GetCollider(col_refs_[i]);

          const auto& shape = world_.GetCollider(col_refs_[i]).Shape;

          switch (shape.index()) {
            case static_cast<int>(Math::ShapeType::Circle):
              if (col.BodyRef == ball_body_ref_) {
                auto& ballBody = world_.GetBody(col.BodyRef);
                ballBody.ApplyForce({0, kBallGravity});

              } else if (col.BodyRef == player_blue_body_ref_) {
                auto& playerBody = world_.GetBody(col.BodyRef);

                if (playerBody.Velocity.X > kMaxSpeed) {
                  playerBody.Velocity.X = kMaxSpeed;
                } else if (playerBody.Velocity.X < -kMaxSpeed) {
                  playerBody.Velocity.X = -kMaxSpeed;
                }
                playerBody.ApplyForce({0, kPlayerGravity});

                // simulate friction with fground
                if (player_nbr == 1) {
                  if (!(input_ & input::kRight) && !(input_ & input::kLeft) &&
                      is_player_blue_grounded_) {
                    playerBody.Velocity = playerBody.Velocity.Lerp(
                        Math::Vec2F::Zero(), 1.f / 10.f);
                  }
                } else {
                  if (!(other_player_input_ & input::kRight) &&
                      !(other_player_input_ & input::kLeft) &&
                      is_player_blue_grounded_) {
                    playerBody.Velocity = playerBody.Velocity.Lerp(
                        Math::Vec2F::Zero(), 1.f / 10.f);
                  }
                }
              } else if (col.BodyRef == player_red_body_ref_) {
                auto& playerBody = world_.GetBody(col.BodyRef);

                if (playerBody.Velocity.X > kMaxSpeed) {
                  playerBody.Velocity.X = kMaxSpeed;
                } else if (playerBody.Velocity.X < -kMaxSpeed) {
                  playerBody.Velocity.X = -kMaxSpeed;
                }
                playerBody.ApplyForce({0, kPlayerGravity});

                // simulate friction with ground
                if (player_nbr == 2) {
                  if (!(input_ & input::kRight) && !(input_ & input::kLeft) &&
                      is_player_red_grounded_) {
                    playerBody.Velocity = playerBody.Velocity.Lerp(
                        Math::Vec2F::Zero(), 1.f / 10.f);
                  }
                } else {
                  if (!(other_player_input_ & input::kRight) &&
                      !(other_player_input_ & input::kLeft) &&
                      is_player_red_grounded_) {
                    playerBody.Velocity = playerBody.Velocity.Lerp(
                        Math::Vec2F::Zero(), 1.f / 10.f);
                  }
                }
                
              }
              break;
            case static_cast<int>(Math::ShapeType::Rectangle):
              break;
            default:
              break;
          }
        }
        timer_.Tick();
        world_.Update(kFixedDeltaTime);
        break;
      default:;
    }
    time -= kFixedDeltaTime;
  }
}

void Game::TearDown() noexcept {
  body_refs_.clear();
  col_refs_.clear();
  world_.TearDown();
}

void Game::StartGame() {
  Setup();
  state_ = GameState::kInGame;
}

GameState Game::GetState() { return state_; }

void Game::SetInput(input::Input input) { input_ = input; }
void Game::SetOtherInput(input::Input input) { other_player_input_ = input; }

float Game::GetBallRadius() const noexcept { return ball_radius_; }

Math::Vec2F Game::GetBallPosition() noexcept {
  return world_.GetBody(ball_body_ref_).Position;
}

Math::Vec2F Game::GetBallVelocity() noexcept {
  return world_.GetBody(ball_body_ref_).Velocity;
}

BallType Game::GetBallType() noexcept { return ball_type_; }

Math::Vec2F Game::GetPlayerBluePos() noexcept {
  return world_.GetBody(player_blue_body_ref_).Position;
}
Math::Vec2F Game::GetPlayerRedPos() noexcept {
  return world_.GetBody(player_red_body_ref_).Position;
}

void Game::OnCollisionEnter(ColliderRef col1, ColliderRef col2) noexcept {
#ifdef TRACY_ENABLE
  ZoneScoped;
#endif
  if ((col1 == player_blue_col_ref_ && col2 == ground_col_ref_) ||
      (col2 == player_blue_col_ref_ && col1 == ground_col_ref_)) {
    is_player_blue_grounded_ = true;
  } else if ((col1 == player_red_col_ref_ && col2 == ground_col_ref_) ||
             (col2 == player_red_col_ref_ && col1 == ground_col_ref_)) {
    is_player_red_grounded_ = true;
  }
}

void Game::CreateBall() noexcept {
  ball_type_ =
      BallType::kBasketball;  // static_cast<BallType>( GetRandomValue(0,
                              // static_cast<int>(BallType::kCount) - 1));

  const auto ballBodyRef = world_.CreateBody();
  body_refs_.push_back(ballBodyRef);
  auto& ballBody = world_.GetBody(ballBodyRef);

  ballBody.Position = {metrics::kWindowWidth * 0.5f,
                       metrics::kWindowHeight * 0.5f};

  const auto ballColRef = world_.CreateCollider(ballBodyRef);
  col_refs_.push_back(ballColRef);
  auto& ballCol = world_.GetCollider(ballColRef);
  ballCol.BodyPosition = ballBody.Position;
  ball_body_ref_ = ballBodyRef;

  switch (ball_type_) {
    case BallType::kFootball:
      ballBody.Mass = 1.f;
      ballCol.Restitution = 1.5f;
      ball_radius_ = metrics::kBallRadiusMedium;
      break;
    case BallType::kVolleyball:
      ballBody.Mass = 0.5f;
      ballCol.Restitution = 2.5f;
      ball_radius_ = metrics::kBallRadiusMedium;
      break;
    case BallType::kBasketball:
      ballBody.Mass = 1.f;
      ballCol.Restitution = 1.9f;
      ball_radius_ = metrics::kBallRadiusLarge;
      break;
    case BallType::kTennisball:
      ballBody.Mass = 0.5f;
      ballCol.Restitution = 2.95f;
      ball_radius_ = metrics::kBallRadiusSmall;
      break;
    case BallType::kBaseball:
      ballBody.Mass = 1.f;
      ballCol.Restitution = 1.f;
      ball_radius_ = metrics::kBallRadiusSmall;
      break;
  }
  ballCol.Shape = Math::CircleF(Math::Vec2F::Zero(), ball_radius_);
}

void Game::CreateTerrain() noexcept {
  // Create ground
  const auto groundRef = world_.CreateBody();
  body_refs_.push_back(groundRef);
  auto& groundBody = world_.GetBody(groundRef);
  groundBody.Type = BodyType::STATIC;
  groundBody.Mass = 1;

  groundBody.Position = {metrics::kWindowWidth * 0.5f,
                         metrics::kWindowHeight - metrics::kGroundSize.Y};

  const auto groundColRef = world_.CreateCollider(groundRef);
  col_refs_.push_back(groundColRef);
  auto& groundCol = world_.GetCollider(groundColRef);
  groundCol.Shape =
      Math::RectangleF({-metrics::kWindowWidth * 0.5f, 0},
                       {metrics::kWindowWidth * 0.5f, metrics::kGroundSize.Y});
  groundCol.BodyPosition = groundBody.Position;
  groundCol.Restitution = 0.f;
  ground_col_ref_ = groundColRef;

  // roof
  const auto roofRef = world_.CreateBody();
  body_refs_.push_back(roofRef);
  auto& roofBody = world_.GetBody(roofRef);
  roofBody.Type = BodyType::STATIC;
  roofBody.Mass = 1;

  roofBody.Position = {metrics::kWindowWidth * 0.5f, 0};

  const auto roofColRef = world_.CreateCollider(roofRef);
  col_refs_.push_back(roofColRef);
  auto& roofCol = world_.GetCollider(roofColRef);
  roofCol.Shape = Math::RectangleF({-metrics::kWindowWidth * 0.5f, 0},
                                   {metrics::kWindowWidth * 0.5f, 0});
  roofCol.BodyPosition = roofBody.Position;
  roofCol.Restitution = 0.f;

  // wall left
  const auto leftWallRef = world_.CreateBody();
  body_refs_.push_back(leftWallRef);
  auto& leftWallBody = world_.GetBody(leftWallRef);
  leftWallBody.Type = BodyType::STATIC;
  leftWallBody.Mass = 1;

  leftWallBody.Position = {0, metrics::kWindowHeight * 0.5f};

  const auto leftWallColRef = world_.CreateCollider(leftWallRef);
  col_refs_.push_back(leftWallColRef);
  auto& leftWallCol = world_.GetCollider(leftWallColRef);
  leftWallCol.Shape = Math::RectangleF({0, -metrics::kWindowHeight * 0.5f},
                                       {0, metrics::kWindowHeight * 0.5f});
  leftWallCol.BodyPosition = leftWallBody.Position;
  leftWallCol.Restitution = 0.f;

  // wall right
  const auto rightWallRef = world_.CreateBody();
  body_refs_.push_back(rightWallRef);
  auto& rightWallBody = world_.GetBody(rightWallRef);
  rightWallBody.Type = BodyType::STATIC;
  rightWallBody.Mass = 1;

  rightWallBody.Position = {metrics::kWindowWidth,
                            metrics::kWindowHeight * 0.5f};

  const auto rightWallColRef = world_.CreateCollider(rightWallRef);
  col_refs_.push_back(rightWallColRef);
  auto& rightWallCol = world_.GetCollider(rightWallColRef);
  rightWallCol.Shape = Math::RectangleF({0, -metrics::kWindowHeight * 0.5f},
                                        {0, metrics::kWindowHeight * 0.5f});
  rightWallCol.BodyPosition = rightWallBody.Position;
  rightWallCol.Restitution = 0.f;

  // goal left
  const auto leftGoalRef = world_.CreateBody();
  body_refs_.push_back(leftGoalRef);
  auto& leftGoalBody = world_.GetBody(leftGoalRef);
  leftGoalBody.Type = BodyType::STATIC;
  leftGoalBody.Mass = 1;

  leftGoalBody.Position = {0, metrics::kWindowHeight - metrics::kGroundSize.Y -
                                  metrics::kGoalSize.Y};

  const auto leftGoalColRef = world_.CreateCollider(leftGoalRef);
  col_refs_.push_back(leftGoalColRef);
  auto& leftGoalCol = world_.GetCollider(leftGoalColRef);

  leftGoalCol.Shape =
      Math::RectangleF({-metrics::kGoalSize.X, 0},
                       {metrics::kGoalSize.X, metrics::kGoalSize.Y / 10.f});

  leftGoalCol.BodyPosition = leftGoalBody.Position;
  leftGoalCol.Restitution = 0.f;

  // goal right
  const auto rightGoalRef = world_.CreateBody();
  body_refs_.push_back(rightGoalRef);
  auto& rightGoalBody = world_.GetBody(rightGoalRef);
  rightGoalBody.Type = BodyType::STATIC;
  rightGoalBody.Mass = 1;

  rightGoalBody.Position = {
      metrics::kWindowWidth,
      metrics::kWindowHeight - metrics::kGroundSize.Y - metrics::kGoalSize.Y};

  const auto rightGoalColRef = world_.CreateCollider(rightGoalRef);
  col_refs_.push_back(rightGoalColRef);
  auto& rightGoalCol = world_.GetCollider(rightGoalColRef);

  rightGoalCol.Shape =
      Math::RectangleF({-metrics::kGoalSize.X, 0},
                       {metrics::kGoalSize.X, metrics::kGoalSize.Y / 10.f});

  rightGoalCol.BodyPosition = rightGoalBody.Position;
  rightGoalCol.Restitution = 0.f;
}

void Game::CreatePlayers() noexcept {
  // player blue
  const auto p1BodyRef = world_.CreateBody();
  body_refs_.push_back(p1BodyRef);
  auto& p1Body = world_.GetBody(p1BodyRef);

  p1Body.Mass = 1;

  p1Body.Position = {metrics::kWindowWidth * 0.33f,
                     metrics::kWindowHeight * 0.66f};

  const auto p1ColRef = world_.CreateCollider(p1BodyRef);
  col_refs_.push_back(p1ColRef);
  auto& p1Col = world_.GetCollider(p1ColRef);
  p1Col.Shape = Math::CircleF(Math::Vec2F::Zero(), metrics::kPlayerRadius);
  p1Col.BodyPosition = p1Body.Position;
  p1Col.Restitution = 0.f;
  player_blue_body_ref_ = p1BodyRef;
  player_blue_col_ref_ = p1ColRef;

  // feets
  // const auto p1FeetsBodyRef = _world.CreateBody();
  //_body_refs.push_back(p1FeetsBodyRef);
  // auto& p1FeetsBody = _world.GetBody(p1FeetsBodyRef);

  // p1FeetsBody.Mass = 1;

  // p1FeetsBody.Position = {metrics::PlayerBluePos.X,
  //                         metrics::PlayerBluePos.Y + metrics::FeetHeight};

  // const auto p1FeetsColRef = _world.CreateCollider(p1FeetsBodyRef);
  //_col_refs.push_back(p1FeetsColRef);
  // auto& p1FeetsCol = _world.GetCollider(p1FeetsColRef);
  // p1FeetsCol.Shape =
  //     Math::RectangleF({-metrics::kPlayerRadius, 0},
  //                      {metrics::kPlayerRadius, metrics::FeetHeight});
  // p1FeetsCol.BodyPosition = p1FeetsBody.Position;
  // p1FeetsCol.Restitution = 0.f;
  //_player_blue_feet_col_ref = p1FeetsBodyRef;

  // player red

  const auto p2BodyRef = world_.CreateBody();
  body_refs_.push_back(p2BodyRef);
  auto& p2Body = world_.GetBody(p2BodyRef);

  p2Body.Mass = 1;

  p2Body.Position = {metrics::kWindowWidth - metrics::kWindowWidth * 0.33f,
                     metrics::kWindowHeight * 0.66f};

  const auto p2ColRef = world_.CreateCollider(p2BodyRef);
  col_refs_.push_back(p2ColRef);
  auto& p2Col = world_.GetCollider(p2ColRef);
  p2Col.Shape = Math::CircleF(Math::Vec2F::Zero(), metrics::kPlayerRadius);
  p2Col.BodyPosition = p2Body.Position;
  p2Col.Restitution = 0.f;
  player_red_body_ref_ = p2BodyRef;
  player_red_col_ref_ = p2ColRef;
}
