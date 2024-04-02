#pragma once
#include "raylib.h"
#include "raymath.h"

enum class Offset {
  Center,
  TopLeft,
  TopRight,
  DownLeft,
  DownRight,
  DownCenter,
  BackGround
};
class Image2D {
 public:
  Texture2D Tex;
  Vector2 Pos;
  Vector2 Origin;
  Rectangle Source;
  Rectangle Dest;

  void Setup(const char* path, float scale, Offset = Offset::Center);
  void TearDown();
  void Draw(Vector2 position);
};