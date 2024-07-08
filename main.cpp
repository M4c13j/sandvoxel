#include "raylib.h"
#include <stdio.h>
#include <string>

int main(int argc, char** argv)
{
    // Initialize raylib ======================
    InitWindow(1200, 700, "Raylib + ImGui Example");
    SetTargetFPS(60);

    // Main loop ==============================
    while (!WindowShouldClose())
    {
        // Rendering
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("This is a raylib window with ImGui!", 10, 10, 20, DARKGRAY);
        DrawText(TextFormat("Fps: %d", GetFPS()), 10, 30, 20, DARKGRAY);
        // Rendering
        EndDrawing();
    }
    std::string dawg;

    CloseWindow();

    return 0;
}
