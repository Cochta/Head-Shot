#include "Image2D.h"

void Image2D::Setup(const char* path, float scale, Offset offset) {
  Tex = LoadTexture(path);
  Source = {0, 0, static_cast<float>(Tex.width),
            static_cast<float>(Tex.height)};

  switch (offset) {
    case Offset::Center:
      Origin = {Tex.width * scale * 0.5f, Tex.height * scale * 0.5f};
      break;
    case Offset::TopLeft:
      Origin = {0,0};
      break;
    case Offset::TopRight:
      Origin = {Tex.width * scale, 0};
      break;
    case Offset::DownLeft:
      Origin = {0, Tex.height * scale};
      break;
    case Offset::DownRight:
      Origin = {Tex.width * scale, Tex.height * scale};
      break;
    case Offset::BackGround:
      break;
    default:
      break;
  }
  
  Dest = {0, 0, Source.width * scale, Source.height * scale};
}

void Image2D::TearDown() { UnloadTexture(Tex); }

void Image2D::Draw(Vector2 position) {
  Dest.x = position.x;
  Dest.y = position.y;
  DrawTexturePro(Tex, Source, Dest, Origin, 0, WHITE);
}