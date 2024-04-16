#pragma once

#include "raylib_wrapper.h"

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
  raylib::Texture2D tex;
  raylib::Vector2 pos;
  raylib::Vector2 origin;
  raylib::Rectangle source;
  raylib::Rectangle dest;

  void Setup(const char* path, raylib::Vector2 size, Offset = Offset::Center);
  void TearDown();
  void Draw(raylib::Vector2 position, float rotation = 0);
};