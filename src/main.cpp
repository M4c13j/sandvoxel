#include "raylib.h"
#include "raymath.h"
#include <cstdio>
#include <string>
#include <map>
#include <cassert>
#include "player.hpp"
#include "chunk.hpp"
#include "world.hpp"
#include "config.hpp"
#include "benchmark.hpp"

// #define ANKERL_NANOBENCH_IMPLEMENT
// #include "nanobench.h"

int main(int argc, char** argv)
{
    // Initialize raylib ======================
    InitWindow(1200, 700, "Raylib + ImGui Example");
    SetTargetFPS(60);
    SetWindowState(FLAG_WINDOW_RESIZABLE ); //| FLAG_BORDERLESS_WINDOWED_MODE);
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT); // Anti-aliasing and V-Sync


    Texture dirt_plank = LoadTexture("../resources/textures/dirt_plank.png");
    Player player = Player();

    Chunk chunk = Chunk({0, -config::CHUNK_HEIGHT/2,0}, 0);
    // chunk.generate_default_blocks(config::CHUNK_HEIGHT / 2);
    chunk.generate_perlin(2137u);
    chunk.update_visibility();
    chunk.generate_mesh();

    Block bb = Block(Block::DirtPlank, 0,0);

    World world = World(3);

    Benchmark bench("Chunk mesh ", 1);
    bench.start();
        world.generate_perlin_chunks(2137u);
        world.mesh_all_chunks();
    bench.stop(world.side * world.side);


    // printf("DEBUGGER\n"); return 0;
    // Main loop ==============================
    while (!WindowShouldClose()) {
        UpdateCamera(&player.camera, CAMERA_FREE);
        // Rendering
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // for (int i=0;i<5;i++) chunk.generate_mesh();

        Ray ray;//player.camera.position, player.camera.target);
        ray.direction = player.camera.target;
        ray.position = player.camera.position;

        BeginMode3D(player.camera);
            for (auto row : world.chunks) {
               for (auto ch : row) {
                   RayCollision collision= GetRayCollisionMesh(ray, ch.chunkMesh, MatrixIdentity());
                   if (collision.hit) {
                       std::cout << "MESH HIT\n";
                       DrawBoundingBox(GetMeshBoundingBox(ch.chunkMesh), BLUE);
                   }
               }
            }
            DrawCubeWiresV((Vector3){ 0.0f, 0.5f, 1.0f }, (Vector3){ 1.0f, 1.0f, 1.0f }, RED);
            DrawCubeV((Vector3){ 0.0f, 0.5f, 1.0f }, (Vector3){ 1.0f, 1.0f, 1.0f }, PURPLE);
            DrawCubeWiresV((Vector3){ 0.0f, 0.5f, -1.0f }, (Vector3){ 1.0f, 1.0f, 1.0f }, DARKGREEN);
            DrawCubeV((Vector3) { 0.0f, 0.5f, -1.0f }, (Vector3){ 1.0f, 3.0f, 1.0f }, YELLOW);
            DrawGrid(100, 1.0f);

            chunk.draw_chunk(dirt_plank);
            world.draw_all(dirt_plank);

            DrawBoundingBox(GetMeshBoundingBox(chunk.chunkMesh), BLACK);
         EndMode3D();

         // debug stays
         DrawText("This is a raylib window with ImGui!", 10, 10, 20, DARKGRAY);
         DrawText(TextFormat("Fps: %d  |  Frame time: %.2fms", GetFPS(), GetFrameTime()*1000), 10, 30, 20, DARKGRAY);
         DrawText(TextFormat("Camera Position: [%.2f, %.2f, %.2f]", player.camera.position.x, player.camera.position.y, player.camera.position.z), 10, 50, 20, DARKGRAY);
         DrawText(TextFormat("  Camera target: [%.2f, %.2f, %.2f]", player.camera.target.x, player.camera.target.y, player.camera.target.z), 10, 70, 20, DARKGRAY);
         // DrawText(TextFormat("Mesh Count: %d", model.meshCount), 10, 90, 20, DARKGRAY);
         // DrawText(TextFormat("Material Count: %d", model.materialCount), 10, 110, 20, DARKGRAY);

         // Rendering
         EndDrawing();
    }


    bench.results();
    CloseWindow();
    // UnloadTexture(planktxt);
    // UnloadModel(model);


    return 0;
}
