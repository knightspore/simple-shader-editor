#include "raylib.h"

#define WIDTH 500
#define HEIGHT 500

// PREVIEW
// 1. Setup cube to rotate
typedef struct {
  Vector3 position;
  float size;
  Color color;
} Cube;
// 2. rotate function
// 3. Create 3D Viewport to render cube

// SHADERS
// 1. Setup shader constants
// 2. Load / Unload - Refresh mechanisms
// 3. Render cube with shader
// 4. File watchers setup for shader files
// 5. Setup hot reloading

int main(void) {
  InitWindow(WIDTH, HEIGHT, "[shadereditor]");

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("Shader text here...", 10, 10, 20, RAYWHITE);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
