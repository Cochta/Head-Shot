#pragma once

#include "raylib.h"

class Audio {
 public:
  Sound music;

  void Setup();

  void TearDown();
};