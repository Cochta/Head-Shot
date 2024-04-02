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
  Texture2D tex;
  Vector2 pos;
  Vector2 origin;
  Rectangle source;
  Rectangle dest;

  void Setup(const char* path, float scale, Offset = Offset::Center);
  void TearDown();
  void Draw(Vector2 position);
};