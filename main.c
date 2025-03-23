#include "raylib.h"
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

#define WIDTH 500
#define HEIGHT 500
#define VERTEX_SHADER_PATH "shaders/shader.vs"
#define FRAGMENT_SHADER "shaders/shader.fs"

// SHADERS
// TODO: Implement file watcher + hot reload

bool shaderLoaded = false;
char *vertexShader;
char *fragmentShader;
Shader shader;
int shaderLocUTime;

int lastModTimeVs;
int lastModTimeFs;

void ClearShaders() {
  UnloadShader(shader);
  shaderLoaded = false;
}

void ReloadShaders() {
  vertexShader = LoadFileText(VERTEX_SHADER_PATH);
  fragmentShader = LoadFileText(FRAGMENT_SHADER);

  if (shaderLoaded) {
    ClearShaders();
  }

  shader = LoadShaderFromMemory(vertexShader, fragmentShader);
  shaderLoaded = true;

  shaderLocUTime = GetShaderLocation(shader, "utime");
}

void CheckForShaderUpdate() {
  struct stat fStatVs;
  struct stat fStatFs;
  int statvs = stat(VERTEX_SHADER_PATH, &fStatVs);
  int statfs = stat(FRAGMENT_SHADER, &fStatFs);

  if (statvs == -1 || statfs == -1) {
    return;
  }

  if (lastModTimeFs != fStatFs.st_mtime || fStatVs.st_mtime != lastModTimeVs) {
    printf("::> Reloading shaders\n");
    ReloadShaders();
    lastModTimeFs = fStatFs.st_mtime;
    lastModTimeVs = fStatVs.st_mtime;
    return;
  };
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
  DrawText(fragmentShader, 10, 10, 12, Fade(BLUE, 0.5f));
  DrawFPS(10, WIDTH - 30);
}

int main(void) {
  SetTraceLogLevel(LOG_TRACE);
  SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
  InitWindow(WIDTH, HEIGHT, "[shadereditor]");
  SetWindowState(FLAG_WINDOW_HIGHDPI);
  SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
  ReloadShaders();

  while (!WindowShouldClose()) {
    CheckForShaderUpdate();
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
