#include "application.h";

int main() {
  Application app;

  app.Setup();

  app.Run();

  app.TearDown();

  return EXIT_SUCCESS;
}