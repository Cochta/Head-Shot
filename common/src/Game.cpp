#include "Game.h"

void Game::ProcessInputP1(Input input) noexcept {
  switch (input) {
    case Input::Right:
      _world.GetBody(_player_blue_body_ref).ApplyForce({kWalkSpeed, 0});
      break;
    case Input::Left:
      _world.GetBody(_player_blue_body_ref).ApplyForce({-kWalkSpeed, 0});
      break;
    case Input::Jump:
      if (_is_player_blue_grounded) {
        _world.GetBody(_player_blue_body_ref).ApplyForce({0, kJumpSpeed});
        _is_player_blue_grounded = false;
      }
      break;
  }
}

void Game::ProcessInputP2(Input input) noexcept {
  switch (input) {
    case Input::Right:
      _world.GetBody(_player_red_body_ref).ApplyForce({kWalkSpeed, 0});
      break;
    case Input::Left:
      _world.GetBody(_player_red_body_ref).ApplyForce({-kWalkSpeed, 0});
      break;
    case Input::Jump:
      if (_is_player_red_grounded) {
        _world.GetBody(_player_red_body_ref).ApplyForce({0, kJumpSpeed});
        _is_player_red_grounded = false;
      }
      break;
  }
}

void Game::Setup() noexcept {
  _timer.SetUp();
  _world.SetUp();
  _world.SetContactListener(this);
  CreateBall();
  CreateTerrain();
  CreatePlayers();
}

void Game::Update() noexcept {
  for (std::size_t i = 0; i < _col_refs.size(); ++i) {
    const auto& col = _world.GetCollider(_col_refs[i]);

    const auto& shape = _world.GetCollider(_col_refs[i]).Shape;

    switch (shape.index()) {
      case static_cast<int>(Math::ShapeType::Circle):
        if (col.BodyRef == _ball_body_ref) {
          auto& ballBody = _world.GetBody(col.BodyRef);
          ballBody.ApplyForce({0, kBallGravity});
          game_data::BallPos = ballBody.Position;
          game_data::BallVelocity = ballBody.Velocity;

        } else if (col.BodyRef == _player_blue_body_ref) {
          auto& playerBody = _world.GetBody(col.BodyRef);

          if (playerBody.Velocity.X > kMaxSpeed) {
            playerBody.Velocity.X = kMaxSpeed;
          } else if (playerBody.Velocity.X < -kMaxSpeed) {
            playerBody.Velocity.X = -kMaxSpeed;
          }

          playerBody.ApplyForce({0, kPlayerGravity});
          game_data::PlayerBluePos = playerBody.Position;
        } else if (col.BodyRef == _player_red_body_ref) {
          auto& playerBody = _world.GetBody(col.BodyRef);

          if (playerBody.Velocity.X > kMaxSpeed) {
            playerBody.Velocity.X = kMaxSpeed;
          } else if (playerBody.Velocity.X < -kMaxSpeed) {
            playerBody.Velocity.X = -kMaxSpeed;
          }

          playerBody.ApplyForce({0, kPlayerGravity});
          game_data::PlayerRedPos = playerBody.Position;
        }
        break;
      case static_cast<int>(Math::ShapeType::Rectangle):
        break;
      default:
        break;
    }
  }
  _timer.Tick();
  _world.Update(1 / 60.f);
}

void Game::TearDown() noexcept {
  _body_refs.clear();
  _col_refs.clear();
  _world.TearDown();
}

void Game::OnCollisionEnter(ColliderRef col1, ColliderRef col2) noexcept {
  if ((col1 == _player_blue_col_ref && col2 == _ground_col_ref) ||
      (col2 == _player_blue_col_ref && col1 == _ground_col_ref)) {
    _is_player_blue_grounded = true;
  } else if ((col1 == _player_red_col_ref && col2 == _ground_col_ref) ||
             (col2 == _player_red_col_ref && col1 == _ground_col_ref)) {
    _is_player_red_grounded = true;
  }
}

void Game::CreateBall() noexcept {
  const auto ballBodyRef = _world.CreateBody();
  _body_refs.push_back(ballBodyRef);
  auto& ballBody = _world.GetBody(ballBodyRef);

  ballBody.Position = game_data::BallPos;

  const auto ballColRef = _world.CreateCollider(ballBodyRef);
  _col_refs.push_back(ballColRef);
  auto& ballCol = _world.GetCollider(ballColRef);
  ballCol.Shape = Math::CircleF(Math::Vec2F::Zero(), metrics::BallRadius);
  ballCol.BodyPosition = ballBody.Position;

  _ball_body_ref = ballBodyRef;

  switch (game_data::Type) {
    case game_data::BallType::FOOTBALL:
      ballBody.Mass = 1.f;
      ballCol.Restitution = 1.5f;
      break;
    case game_data::BallType::VOLLEYBALL:
      ballBody.Mass = 0.5f;
      ballCol.Restitution = 2.5f;
      break;
    case game_data::BallType::BASKETBALL:
      ballBody.Mass = 1.f;
      ballCol.Restitution = 1.9f;
      break;
    case game_data::BallType::TENNISBALL:
      ballBody.Mass = 0.5f;
      ballCol.Restitution = 2.95f;
      break;
    case game_data::BallType::BASEBALL:
      ballBody.Mass = 1.f;
      ballCol.Restitution = 1.f;
      break;
  }
}

void Game::CreateTerrain() noexcept {
  // Create ground
  const auto groundRef = _world.CreateBody();
  _body_refs.push_back(groundRef);
  auto& groundBody = _world.GetBody(groundRef);
  groundBody.Type = BodyType::STATIC;
  groundBody.Mass = 1;

  groundBody.Position = {metrics::Width * 0.5f,
                         metrics::Height - metrics::GroundHeight};

  const auto groundColRef = _world.CreateCollider(groundRef);
  _col_refs.push_back(groundColRef);
  auto& groundCol = _world.GetCollider(groundColRef);
  groundCol.Shape =
      Math::RectangleF({-metrics::Width * 0.5f, 0},
                       {metrics::Width * 0.5f, metrics::GroundHeight});
  groundCol.BodyPosition = groundBody.Position;
  groundCol.Restitution = 0.f;
  _ground_col_ref = groundColRef;

  // roof
  const auto roofRef = _world.CreateBody();
  _body_refs.push_back(roofRef);
  auto& roofBody = _world.GetBody(roofRef);
  roofBody.Type = BodyType::STATIC;
  roofBody.Mass = 1;

  roofBody.Position = {metrics::Width * 0.5f, 0};

  const auto roofColRef = _world.CreateCollider(roofRef);
  _col_refs.push_back(roofColRef);
  auto& roofCol = _world.GetCollider(roofColRef);
  roofCol.Shape =
      Math::RectangleF({-metrics::Width * 0.5f, 0}, {metrics::Width * 0.5f, 0});
  roofCol.BodyPosition = roofBody.Position;
  roofCol.Restitution = 0.f;

  // wall left
  const auto leftWallRef = _world.CreateBody();
  _body_refs.push_back(leftWallRef);
  auto& leftWallBody = _world.GetBody(leftWallRef);
  leftWallBody.Type = BodyType::STATIC;
  leftWallBody.Mass = 1;

  leftWallBody.Position = {0, metrics::Height * 0.5f};

  const auto leftWallColRef = _world.CreateCollider(leftWallRef);
  _col_refs.push_back(leftWallColRef);
  auto& leftWallCol = _world.GetCollider(leftWallColRef);
  leftWallCol.Shape = Math::RectangleF({0, -metrics::Height * 0.5f},
                                       {0, metrics::Height * 0.5f});
  leftWallCol.BodyPosition = leftWallBody.Position;
  leftWallCol.Restitution = 0.f;

  // wall right
  const auto rightWallRef = _world.CreateBody();
  _body_refs.push_back(rightWallRef);
  auto& rightWallBody = _world.GetBody(rightWallRef);
  rightWallBody.Type = BodyType::STATIC;
  rightWallBody.Mass = 1;

  rightWallBody.Position = {metrics::Width, metrics::Height * 0.5f};

  const auto rightWallColRef = _world.CreateCollider(rightWallRef);
  _col_refs.push_back(rightWallColRef);
  auto& rightWallCol = _world.GetCollider(rightWallColRef);
  rightWallCol.Shape = Math::RectangleF({0, -metrics::Height * 0.5f},
                                        {0, metrics::Height * 0.5f});
  rightWallCol.BodyPosition = rightWallBody.Position;
  rightWallCol.Restitution = 0.f;

  // goal left
  const auto leftGoalRef = _world.CreateBody();
  _body_refs.push_back(leftGoalRef);
  auto& leftGoalBody = _world.GetBody(leftGoalRef);
  leftGoalBody.Type = BodyType::STATIC;
  leftGoalBody.Mass = 1;

  leftGoalBody.Position = {
      0, metrics::Height - metrics::GroundHeight - metrics::GoalSize.Y};

  const auto leftGoalColRef = _world.CreateCollider(leftGoalRef);
  _col_refs.push_back(leftGoalColRef);
  auto& leftGoalCol = _world.GetCollider(leftGoalColRef);
  leftGoalCol.Shape = Math::CircleF(metrics::GoalSize.X);
  leftGoalCol.BodyPosition = leftGoalBody.Position;
  leftGoalCol.Restitution = 0.f;

  // goal right
  const auto rightGoalRef = _world.CreateBody();
  _body_refs.push_back(rightGoalRef);
  auto& rightGoalBody = _world.GetBody(rightGoalRef);
  rightGoalBody.Type = BodyType::STATIC;
  rightGoalBody.Mass = 1;

  rightGoalBody.Position = {
      metrics::Width,
      metrics::Height - metrics::GroundHeight - metrics::GoalSize.Y};

  const auto rightGoalColRef = _world.CreateCollider(rightGoalRef);
  _col_refs.push_back(rightGoalColRef);
  auto& rightGoalCol = _world.GetCollider(rightGoalColRef);
  rightGoalCol.Shape = Math::CircleF(metrics::GoalSize.X);
  rightGoalCol.BodyPosition = rightGoalBody.Position;
  rightGoalCol.Restitution = 0.f;
}

void Game::CreatePlayers() noexcept {
  // player blue
  const auto p1BodyRef = _world.CreateBody();
  _body_refs.push_back(p1BodyRef);
  auto& p1Body = _world.GetBody(p1BodyRef);

  p1Body.Mass = 1;

  p1Body.Position = game_data::PlayerBluePos;

  const auto p1ColRef = _world.CreateCollider(p1BodyRef);
  _col_refs.push_back(p1ColRef);
  auto& p1Col = _world.GetCollider(p1ColRef);
  p1Col.Shape = Math::CircleF(Math::Vec2F::Zero(), metrics::PlayerRadius);
  p1Col.BodyPosition = p1Body.Position;
  p1Col.Restitution = 0.f;
  _player_blue_body_ref = p1BodyRef;
  _player_blue_col_ref = p1ColRef;

  // feets
  //const auto p1FeetsBodyRef = _world.CreateBody();
  //_body_refs.push_back(p1FeetsBodyRef);
  //auto& p1FeetsBody = _world.GetBody(p1FeetsBodyRef);

  //p1FeetsBody.Mass = 1;

  //p1FeetsBody.Position = {metrics::PlayerBluePos.X,
  //                        metrics::PlayerBluePos.Y + metrics::FeetHeight};

  //const auto p1FeetsColRef = _world.CreateCollider(p1FeetsBodyRef);
  //_col_refs.push_back(p1FeetsColRef);
  //auto& p1FeetsCol = _world.GetCollider(p1FeetsColRef);
  //p1FeetsCol.Shape =
  //    Math::RectangleF({-metrics::PlayerRadius, 0},
  //                     {metrics::PlayerRadius, metrics::FeetHeight});
  //p1FeetsCol.BodyPosition = p1FeetsBody.Position;
  //p1FeetsCol.Restitution = 0.f;
  //_player_blue_feet_col_ref = p1FeetsBodyRef;

  // player red

  const auto p2BodyRef = _world.CreateBody();
  _body_refs.push_back(p2BodyRef);
  auto& p2Body = _world.GetBody(p2BodyRef);

  p2Body.Mass = 1;

  p2Body.Position = game_data::PlayerRedPos;

  const auto p2ColRef = _world.CreateCollider(p2BodyRef);
  _col_refs.push_back(p2ColRef);
  auto& p2Col = _world.GetCollider(p2ColRef);
  p2Col.Shape = Math::CircleF(Math::Vec2F::Zero(), metrics::PlayerRadius);
  p2Col.BodyPosition = p2Body.Position;
  p2Col.Restitution = 0.f;
  _player_red_body_ref = p2BodyRef;
  _player_red_col_ref = p2ColRef;
}
