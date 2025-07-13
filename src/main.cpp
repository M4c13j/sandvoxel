#include "Block/Air.hpp"
#include "Block/Sand.hpp"
#include "benchmark.hpp"
#include "World/chunk.hpp"
#include "config.hpp"
#include "player.hpp"
#include "raylib.h"
#include "raymath.h"
#include "./World/world.hpp"

#include <cassert>
#include <cstdio>
#include <map>
#include <string>

#include <rlgl.h>

// #define ANKERL_NANOBENCH_IMPLEMENT
// #include "nanobench.h"

int main(int argc, char** argv)
{
    // Initialize raylib ======================
    InitWindow(1600, 900, "Raylib + ImGui Example");
    SetTargetFPS(60);
    SetWindowState(FLAG_WINDOW_RESIZABLE ); //| FLAG_BORDERLESS_WINDOWED_MODE);
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT); // Anti-aliasing and V-Sync


    Texture dirt_plank = LoadTexture("resources/textures/dirt_plank.png");

    Player player = Player();
    BlockFactory::getInstance().initBlocks();

    Chunk chunk = Chunk({0, -config::CHUNK_SIZE/2,0}, 0);
    // chunk.generate_default_blocks(config::CHUNK_HEIGHT / 2);
    chunk.generate_perlin(1241241);
    // chunk.generate_mesh();

    World *world = new World(); // stack overfow for bigger worlds if allocated on stack;
    Benchmark bench("Chunk mesh ", 1);
    Benchmark single("Single chunk mesh, with neighbours", 1);
    bench.start();
        world->generate_perlin_chunks(123123);
        world->mesh_all_chunks();
    bench.stop(world->side * world->side * world->height);
    single.start();
        int single_iters = 1000;
        for (int i = 0; i < single_iters; i++) {
            world->get_chunk_raw_access({13, 2, 1}).generate_mesh();
        }
    single.stop(single_iters);


    // Demo timing variables - can be removed in production
    bool skipInitialLag = true;
    float fluidDemoStartTime = GetTime() + 5.0f;
    float sandDemoStartTime = GetTime() + 2.0f;
    float sandDemoStopTime = GetTime() + 100;
    const float fluidDemoStopTime = fluidDemoStartTime + 2;
    
    // OpenGL render settings
    rlSetLineWidth(3.0f); // Make lines more visible

    // Main loop
    while (!WindowShouldClose()) {

        // FLUID DEMO
        if (IsKeyDown(KEY_N)) {
            for (int x = -4; x <= 4; x++) {
                for (int z = 6; z <= 10; z++) {
                    world->fluidSim.addBlock(x-23, 47, z-21);
                }
            }
            fluidDemoStartTime += 0.1f;
        }

        // SAND DEMO
        if (IsKeyDown(KEY_B)) {
            for (int x = -2; x <= 1; x++) {
                for (int z = 6; z <= 8; z++) {
                    world->sandSim.addBlock(x - 10, 15 + 30, z - 10);
                    world->sandSim.addBlock(x - 10, 15 - 30 + 1, z - 10);
                }
            }
            world->sandSim.addBlock(-15 - 10, 30, -15 - 10);
            sandDemoStartTime += 0.1f;
        }

        // USER INPUT FOR BLOCK PLACEMENT
        Cord currentPlayerPos{static_cast<int>(player.camera.position.x), 
                              static_cast<int>(player.camera.position.y),
                              static_cast<int>(player.camera.position.z)};

        if (IsKeyDown(KEY_V)) {
            world->sandSim.addBlock(currentPlayerPos);
        }
        if (IsKeyDown(KEY_M)) {
            world->fluidSim.addBlock(currentPlayerPos);
        }

        // UPDATE WORLD AND SIMULATION
        UpdateCamera(&player.camera, CAMERA_FREE);
        world->update();

        // Wire frame debugger
        if (IsKeyDown(KEY_R))
            rlEnableWireMode();
        else
            rlDisableWireMode();


        Ray ray;//player.camera.position, player.camera.target);
        ray.direction = player.camera.target;
        ray.position = player.camera.position;

        BeginDrawing();
        ClearBackground(RAYWHITE);

            BeginMode3D(player.camera);
                world->draw_all(dirt_plank, DRAW_CHUNK_DEBUG_WIRES_MODEL);
                chunk.draw_chunk(dirt_plank, DRAW_CHUNK_DEBUG_WIRES_MODEL);
            EndMode3D();

             // Debug status display
             DrawRectangleGradientV(0, 0, 350, 120,BLUE, RED);
             DrawText("SandVoxel debug.", 10, 10, 20, BLACK);
             DrawText(TextFormat("Fps: %d  |  Frame time: %.2fms", GetFPS(), GetFrameTime()*1000), 10, 30, 20, BLACK);
             // Optimize: avoid string allocation in hot path by formatting directly
             Cord chunkCord = world->chunk_cord_of_block(player.camera.position);
             DrawText(TextFormat("Chunk: (%d, %d, %d)", chunkCord.x, chunkCord.y, chunkCord.z), 10, 50, 20, BLACK);
             DrawText(TextFormat("pos: [%.2f, %.2f, %.2f]", player.camera.position.x, player.camera.position.y, player.camera.position.z), 10, 70, 20, BLACK);
             DrawText(TextFormat("target: [%.2f, %.2f, %.2f]", player.camera.target.x, player.camera.target.y, player.camera.target.z), 10, 90, 20, BLACK);

         EndDrawing();
    }



    CloseWindow();
    bench.results();
    single.results();
    world->print_size_report();
    delete world;

    return 0;
}
