#pragma once

#include <array>

#include "Image2D.h"
#include "Metrics.h"

class Renderer {
 private:
  static constexpr int font_size = 30;

  static constexpr Rectangle screen = {0, 0, Metrics::Width, Metrics::Height};

 public:
  Image2D ball;
  Image2D ground;
  Image2D goalLeft;
  Image2D goalRight;

  void Setup();

  void TearDown();

  void Draw(void);
};