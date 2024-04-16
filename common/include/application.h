#pragma once

#include "Audio.h"
#include "Game.h"
#include "Renderer.h"
#include "network.h"

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#else
#endif

class Application {
 public:
  void Setup();
  void Run();
  void TearDown();

 private:
  Game game_;
  Audio audio_;
  Renderer renderer_;
  Network network_{PHOTON_APP_ID, "V1.0", &game_, &renderer_};

  Timer GameTimer;
  float GameTime = 0;
  int GameFrame = 0;
};