#pragma once

#include "raylib_wrapper.h"

using namespace raylib;

class Audio {
 public:
  Sound music;

  void Setup();

  void TearDown();
};