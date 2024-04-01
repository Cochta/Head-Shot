#include "Metrics.h"
#include "raylib.h"
#include "raymath.h"

class Image2D {
 public:
  Texture2D Tex;
  Vector2 Pos;
  Vector2 Center;
  Rectangle Source;

  Rectangle Dest;

  void Setup(const char* path);
  void TearDown();
  void Draw(Vector2 position, float scale);
};

class Renderer {
 private:
  static constexpr int font_size = 30;

  static constexpr Rectangle screen = {0, 0, Metrics::Width, Metrics::Height};

 public:
  Image2D penguin;

  void Setup();

  void TearDown();

  void Draw(void);
};