#pragma once

#include "raylib_wrapper.h"

using namespace raylib;

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

  void Setup(const char* path, Vector2 size, Offset = Offset::Center);
  void TearDown();
  void Draw(Vector2 position, float rotation = 0);
};