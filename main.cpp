#include "raylib.h"
#include <stdio.h>
#include <string>
#include <map>
int main(int argc, char** argv)
{
    // Initialize raylib ======================
    InitWindow(1200, 700, "Raylib + ImGui Example");
    SetTargetFPS(60);

    Camera camera = {
        .position = (Vector3){ 2.0f, 2.0f, 3.0f },    // Camera position
        .target = (Vector3){ 0.0f, 0.5f, 0.0f },      // Camera looking at point
        .up = (Vector3){ 0.0f, 1.0f, 0.0f },          // Camera up vector (rotation towards target)
        .fovy = 45.0f,                                // Camera field-of-view Y
        .projection = CAMERA_PERSPECTIVE              // Camera projection type
    };

    Mesh mesh = GenMeshPlane(10, 10, 0, 0);
    UploadMesh(&mesh, 0);

    // Main loop ==============================
    while (!WindowShouldClose())
    {
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);
        // Rendering
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("This is a raylib window with ImGui!", 10, 10, 20, DARKGRAY);
        DrawText(TextFormat("Fps: %d", GetFPS()), 10, 30, 20, DARKGRAY);

        BeginMode3D(camera);
                DrawCubeWiresV((Vector3){ 0.0f, 0.5f, 1.0f }, (Vector3){ 1.0f, 1.0f, 1.0f }, RED);
                DrawCubeV((Vector3){ 0.0f, 0.5f, 1.0f }, (Vector3){ 1.0f, 1.0f, 1.0f }, PURPLE);
                DrawCubeWiresV((Vector3){ 0.0f, 0.5f, -1.0f }, (Vector3){ 1.0f, 1.0f, 1.0f }, DARKGREEN);
                DrawCubeV((Vector3) { 0.0f, 0.5f, -1.0f }, (Vector3){ 1.0f, 1.0f, 1.0f }, YELLOW);
                DrawGrid(10, 1.0f);

        EndMode3D();
        // Rendering
        EndDrawing();
    }
    std::string dawg;

    CloseWindow();

    return 0;
}
