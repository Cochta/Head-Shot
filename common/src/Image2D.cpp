#include "Image2D.h"

void Image2D::Setup(const char* path, float scale, Offset offset) {
  tex = LoadTexture(path);
  source = {0, 0, static_cast<float>(tex.width),
            static_cast<float>(tex.height)};

  switch (offset) {
    case Offset::Center:
      origin = {tex.width * scale * 0.5f, tex.height * scale * 0.5f};
      break;
    case Offset::TopLeft:
      origin = {0, 0};
      break;
    case Offset::TopRight:
      origin = {tex.width * scale, 0};
      break;
    case Offset::DownLeft:
      origin = {0, tex.height * scale};
      break;
    case Offset::DownRight:
      origin = {tex.width * scale, tex.height * scale};
      break;
    case Offset::DownCenter:
      origin = {tex.width * scale * 0.5f, tex.height * scale};
      break;
    case Offset::BackGround:
      break;
    default:
      break;
  }

  dest = {0, 0, source.width * scale, source.height * scale};
}

void Image2D::TearDown() { UnloadTexture(tex); }

void Image2D::Draw(Vector2 position, float rotation) {
  dest.x = position.x;
  dest.y = position.y;
  DrawTexturePro(tex, source, dest, origin, rotation, WHITE);
}