#include "raylib.h"

#define WIDTH 500
#define HEIGHT 500
#define VERTEX_SHADER "shaders/shader.vs"
#define FRAGMENT_SHADER "shaders/shader.fs"

// SHADERS
// TODO: Implement file watcher + hot reload

bool shaderLoaded = false;
bool shaderUpdated = false; // TODO: Implement
char *vertexShader;
char *fragmentShader;
Shader shader;
int shaderLocUTime;

void ClearShaders() {
  UnloadShader(shader);
  shaderLoaded = false;
}

void LoadShaders() {
  vertexShader = LoadFileText(VERTEX_SHADER);
  fragmentShader = LoadFileText(FRAGMENT_SHADER);

  if (shaderLoaded) {
    ClearShaders();
  }

  shader = LoadShaderFromMemory(vertexShader, fragmentShader);
  shaderLoaded = true;

  shaderLocUTime = GetShaderLocation(shader, "utime");
}

// Cube

Vector3 cubePos = {0.0f, 0.0f, 0.0f};

Camera camera = {
    .position = {10.0f, 10.0f, 10.0f},
    .target = {0.0f, 0.0f, 0.0f},
    .up = {0.0f, 1.0f, 0.0f},
    .fovy = 45.0f,
    .projection = CAMERA_PERSPECTIVE,
};

void RenderCube() {
  if (shaderUpdated) {
    // ReloadShaders();
  }

  float time = GetTime();

  UpdateCamera(&camera, CAMERA_ORBITAL);
  BeginMode3D(camera);

  SetShaderValue(shader, shaderLocUTime, &time, SHADER_UNIFORM_FLOAT);
  BeginShaderMode(shader);

  DrawCube(cubePos, 2.0f, 2.0f, 2.0f, RED);
  DrawCubeWires(cubePos, 2.0f, 2.0f, 2.0f, WHITE);

  EndShaderMode();

  EndMode3D();
}

// UTILS

void RenderText() {
  DrawText(fragmentShader, 10, 10, 10, Fade(BLUE, 0.5f));
  DrawFPS(10, WIDTH - 30);
}

int main(void) {
  SetWindowState(FLAG_MSAA_4X_HINT);
  InitWindow(WIDTH, HEIGHT, "[shadereditor]");
  SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
  LoadShaders();

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLANK);

    RenderCube();
    RenderText();

    EndDrawing();
  }

  if (shaderLoaded) {
    ClearShaders();
  }
  CloseWindow();

  return 0;
}
