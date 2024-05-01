#pragma once

#include "raylib_wrapper.h"

using namespace raylib;

class Audio {
 public:
  Music music;

  void Setup();

	void Update();

  void TearDown();
};