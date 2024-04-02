#pragma once

#include <array>

#include "Image2D.h"
#include "Metrics.h"

class Renderer {
 private:
  static constexpr int kFontSize = 30;

  //static constexpr Rectangle screen = {0, 0, Metrics::Width, Metrics::Height};

 public:
  Image2D ball;
  Image2D ground;
  Image2D goal_left;
  Image2D goal_right;
  Image2D player_blue;
  Image2D player_red;

  void Setup();

  void TearDown();

  void Draw(void);
  void DrawDebug();
};