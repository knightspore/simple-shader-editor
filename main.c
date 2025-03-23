#include "raylib.h"
#include <stdio.h>
#include <sys/stat.h>

#define WIDTH 500
#define HEIGHT 500
#define VERTEX_SHADER_PATH "shaders/shader.vs"
#define FRAGMENT_SHADER "shaders/shader.fs"

// SHADERS

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

// Preview

Vector3 cubePos = {0.0f, 0.0f, 0.0f};

Camera camera = {
    .position = {10.0f, 10.0f, 10.0f},
    .target = {0.0f, 0.0f, 0.0f},
    .up = {0.0f, 1.0f, 0.0f},
    .fovy = 45.0f,
    .projection = CAMERA_PERSPECTIVE,
};

void RenderPreview() {
  float time = GetTime();

  UpdateCamera(&camera, CAMERA_ORBITAL);
  BeginMode3D(camera);

  SetShaderValue(shader, shaderLocUTime, &time, SHADER_UNIFORM_FLOAT);
  BeginShaderMode(shader);

  DrawSphereWires(cubePos, 1, 10, 10, RED);

  EndShaderMode();

  EndMode3D();
}

// UTILS

int textSize = 10;
Vector2 textOrigin = {10, 10};
Vector2 lastMousePos = {0, 0};

void TrackTextPosition() {
  if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)) {
    if (lastMousePos.x != 0 && lastMousePos.y != 0) {
      textOrigin.x += GetMouseX() - lastMousePos.x;
      textOrigin.y += GetMouseY() - lastMousePos.y;
    }
    lastMousePos = (Vector2){GetMouseX(), GetMouseY()};
  } else {
    lastMousePos = (Vector2){0, 0};
  }

  if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_EQUAL)) {
    if (textSize > 50)
      return;
    textSize += 1;
  }

  if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_MINUS)) {
    if (textSize < 10)
      return;
    textSize -= 1;
  }
}

void RenderText() {
  TrackTextPosition();

  int fsW = MeasureText(fragmentShader, textSize);
  DrawText(fragmentShader, textOrigin.x, textOrigin.y, textSize, RAYWHITE);
  DrawText(vertexShader, fsW + textOrigin.x + textSize, textOrigin.y, textSize,
           RAYWHITE);

  const char *fps = TextFormat("FPS %i", GetFPS());
  int fpsW = MeasureText(fps, 10);
  DrawText(fps, GetScreenWidth() - fpsW - 10, 10, 10, RAYWHITE);
}

int main(void) {
  SetTraceLogLevel(LOG_NONE);
  SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
  SetWindowState(FLAG_WINDOW_RESIZABLE);
  InitWindow(WIDTH, HEIGHT, "[shadereditor]");
  SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
  ReloadShaders();

  while (!WindowShouldClose()) {
    SetWindowSize(GetScreenWidth(), GetScreenHeight());
    CheckForShaderUpdate();
    BeginDrawing();
    ClearBackground(BLANK);

    RenderPreview();
    RenderText();

    EndDrawing();
  }

  ClearShaders();
  CloseWindow();

  return 0;
}
