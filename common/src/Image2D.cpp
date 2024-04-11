#include "Image2D.h"

void Image2D::Setup(const char* path, Vector2 size, Offset offset) {
  tex = LoadTexture(path);
  source = {0, 0, static_cast<float>(tex.width),
            static_cast<float>(tex.height)};
  dest = {0, 0, size.x, size.y};

  switch (offset) {
    case Offset::Center:
      origin = {dest.width * 0.5f, dest.height * 0.5f};
      break;
    case Offset::TopLeft:
      origin = {0, 0};
      break;
    case Offset::TopRight:
      origin = {dest.width, 0};
      break;
    case Offset::DownLeft:
      origin = {0, dest.height};
      break;
    case Offset::DownRight:
      origin = {dest.width, dest.height};
      break;
    case Offset::DownCenter:
      origin = {dest.width * 0.5f, dest.height};
      break;
    case Offset::BackGround:
      break;
    default:
      break;
  }
}

void Image2D::TearDown() { UnloadTexture(tex); }

void Image2D::Draw(Vector2 position, float rotation) {
  dest.x = position.x;
  dest.y = position.y;
  DrawTexturePro(tex, source, dest, origin, rotation, WHITE);
}