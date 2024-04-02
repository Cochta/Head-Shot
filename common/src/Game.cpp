#include "Game.h"

void Game::ProcessInput(Input input) noexcept {
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
    default:
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
          _world.GetBody(col.BodyRef).ApplyForce({0, kBallGravity});
          Metrics::BallPos = _world.GetBody(col.BodyRef).Position;
        } else if (col.BodyRef == _player_blue_body_ref) {
          auto& playerbody = _world.GetBody(col.BodyRef);
          if (playerbody.Velocity.X > kMaxSpeed) {
            playerbody.Velocity.X = kMaxSpeed;
          } else if (playerbody.Velocity.X < -kMaxSpeed) {
            playerbody.Velocity.X = -kMaxSpeed;
          }
          playerbody.ApplyForce({0, kPlayerGravity});

          Metrics::PlayerBluePos = playerbody.Position;
        } else if (col.BodyRef == _player_red_body_ref) {
          _world.GetBody(col.BodyRef).ApplyForce({0, kPlayerGravity});
          Metrics::PlayerRedPos = _world.GetBody(col.BodyRef).Position;
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
  }
}

void Game::CreateBall() noexcept {
  const auto ballBodyRef = _world.CreateBody();
  _body_refs.push_back(ballBodyRef);
  auto& ballBody = _world.GetBody(ballBodyRef);

  ballBody.Mass = 1;

  ballBody.Position = Metrics::BallPos;

  const auto ballColRef = _world.CreateCollider(ballBodyRef);
  _col_refs.push_back(ballColRef);
  auto& ballCol = _world.GetCollider(ballColRef);
  ballCol.Shape = Math::CircleF(Math::Vec2F::Zero(), Metrics::BallRadius);
  ballCol.BodyPosition = ballBody.Position;
  ballCol.Restitution = 1.5f;
  _ball_body_ref = ballBodyRef;
}

void Game::CreateTerrain() noexcept {
  // Create ground
  const auto groundRef = _world.CreateBody();
  _body_refs.push_back(groundRef);
  auto& groundBody = _world.GetBody(groundRef);
  groundBody.Type = BodyType::STATIC;
  groundBody.Mass = 1;

  groundBody.Position = {Metrics::Width * 0.5f,
                         Metrics::Height - Metrics::GroundHeight};

  const auto groundColRef = _world.CreateCollider(groundRef);
  _col_refs.push_back(groundColRef);
  auto& groundCol = _world.GetCollider(groundColRef);
  groundCol.Shape =
      Math::RectangleF({-Metrics::Width * 0.5f, 0},
                       {Metrics::Width * 0.5f, Metrics::GroundHeight});
  groundCol.BodyPosition = groundBody.Position;
  groundCol.Restitution = 0.f;
  _ground_col_ref = groundColRef;

  // roof
  const auto roofRef = _world.CreateBody();
  _body_refs.push_back(roofRef);
  auto& roofBody = _world.GetBody(roofRef);
  roofBody.Type = BodyType::STATIC;
  roofBody.Mass = 1;

  roofBody.Position = {Metrics::Width * 0.5f, 0};

  const auto roofColRef = _world.CreateCollider(roofRef);
  _col_refs.push_back(roofColRef);
  auto& roofCol = _world.GetCollider(roofColRef);
  roofCol.Shape =
      Math::RectangleF({-Metrics::Width * 0.5f, 0}, {Metrics::Width * 0.5f, 0});
  roofCol.BodyPosition = roofBody.Position;
  roofCol.Restitution = 0.f;

  // wall left
  const auto leftWallRef = _world.CreateBody();
  _body_refs.push_back(leftWallRef);
  auto& leftWallBody = _world.GetBody(leftWallRef);
  leftWallBody.Type = BodyType::STATIC;
  leftWallBody.Mass = 1;

  leftWallBody.Position = {0, Metrics::Height * 0.5f};

  const auto leftWallColRef = _world.CreateCollider(leftWallRef);
  _col_refs.push_back(leftWallColRef);
  auto& leftWallCol = _world.GetCollider(leftWallColRef);
  leftWallCol.Shape = Math::RectangleF({0, -Metrics::Height * 0.5f},
                                       {0, Metrics::Height * 0.5f});
  leftWallCol.BodyPosition = leftWallBody.Position;
  leftWallCol.Restitution = 0.f;

  // wall right
  const auto rightWallRef = _world.CreateBody();
  _body_refs.push_back(rightWallRef);
  auto& rightWallBody = _world.GetBody(rightWallRef);
  rightWallBody.Type = BodyType::STATIC;
  rightWallBody.Mass = 1;

  rightWallBody.Position = {Metrics::Width, Metrics::Height * 0.5f};

  const auto rightWallColRef = _world.CreateCollider(rightWallRef);
  _col_refs.push_back(rightWallColRef);
  auto& rightWallCol = _world.GetCollider(rightWallColRef);
  rightWallCol.Shape = Math::RectangleF({0, -Metrics::Height * 0.5f},
                                        {0, Metrics::Height * 0.5f});
  rightWallCol.BodyPosition = rightWallBody.Position;
  rightWallCol.Restitution = 0.f;
}

void Game::CreatePlayers() noexcept {
  // player blue
  const auto p1BodyRef = _world.CreateBody();
  _body_refs.push_back(p1BodyRef);
  auto& p1Body = _world.GetBody(p1BodyRef);

  p1Body.Mass = 1;

  p1Body.Position = Metrics::PlayerBluePos;

  const auto p1ColRef = _world.CreateCollider(p1BodyRef);
  _col_refs.push_back(p1ColRef);
  auto& p1Col = _world.GetCollider(p1ColRef);
  p1Col.Shape = Math::CircleF(Math::Vec2F::Zero(), Metrics::PlayerRadius);
  p1Col.BodyPosition = p1Body.Position;
  p1Col.Restitution = 0.f;
  _player_blue_body_ref = p1BodyRef;
  _player_blue_col_ref = p1ColRef;

  // player red

  const auto p2BodyRef = _world.CreateBody();
  _body_refs.push_back(p2BodyRef);
  auto& p2Body = _world.GetBody(p2BodyRef);

  p2Body.Mass = 1;

  p2Body.Position = Metrics::PlayerRedPos;

  const auto p2ColRef = _world.CreateCollider(p2BodyRef);
  _col_refs.push_back(p2ColRef);
  auto& p2Col = _world.GetCollider(p2ColRef);
  p2Col.Shape = Math::CircleF(Math::Vec2F::Zero(), Metrics::PlayerRadius);
  p2Col.BodyPosition = p2Body.Position;
  p2Col.Restitution = 0.f;
  _player_red_body_ref = p2BodyRef;
  _player_red_col_ref = p2ColRef;
}
