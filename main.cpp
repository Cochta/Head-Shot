#include "application.h";

#ifdef TRACY_ENABLE
#include <TracyC.h>

#include <Tracy.hpp>
#endif

#include <iostream>

int main() {
  Application app;

  app.Setup();

  app.Run();

  app.TearDown();

  // std::array<input::Input, 5400> GameInputs; // pour chaque joueur

  // network manager a une reference au renderer et au game

  // pres pk les var glob sont code smell

  return EXIT_SUCCESS;
}