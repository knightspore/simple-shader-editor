#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <stdio.h>

#define WIDTH 500
#define HEIGHT 500

// PREVIEW
// 1. Setup cube to rotate              : DONE
// 2. rotate function                   : DONE
// 3. Create 3D Viewport to render cube : DONE
// 4. Setup camera                      : DONE
// 5. Render cube

Vector3 cubePos = {0.0f, 0.0f, 0.0f};

Camera camera = {
    .position = {10.0f, 10.0f, 10.0f},
    .target = {0.0f, 0.0f, 0.0f},
    .up = {0.0f, 1.0f, 0.0f},
    .fovy = 45.0f,
    .projection = CAMERA_PERSPECTIVE,
};

void UpdateDrawCube3D() {
  UpdateCamera(&camera, CAMERA_ORBITAL);
  BeginMode3D(camera);
  DrawCube(cubePos, 2.0f, 2.0f, 2.0f, RED);
  DrawCubeWires(cubePos, 2.0f, 2.0f, 2.0f, WHITE);
  EndMode3D();
}

// SHADERS
// 1. Setup shader constants
// 2. Load / Unload - Refresh mechanisms
// 3. Render cube with shader
// 4. File watchers setup for shader files
// 5. Setup hot reloading

// UTILS

void DrawDebugText() {
  DrawText("Shader text here...", 10, 10, 10, Fade(WHITE, 0.5f));
  DrawFPS(10, WIDTH - 30);
}

int main(void) {
  InitWindow(WIDTH, HEIGHT, "[shadereditor]");
  SetWindowState(FLAG_MSAA_4X_HINT);
  SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    UpdateDrawCube3D();
    DrawDebugText();

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
