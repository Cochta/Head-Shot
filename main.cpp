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

  return EXIT_SUCCESS;
}