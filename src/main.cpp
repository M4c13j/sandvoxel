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
    InitWindow(1200, 700, "Raylib + ImGui Example");
    SetTargetFPS(60);
    SetWindowState(FLAG_WINDOW_RESIZABLE ); //| FLAG_BORDERLESS_WINDOWED_MODE);
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT); // Anti-aliasing and V-Sync


    Texture dirt_plank = LoadTexture("../resources/textures/dirt_plank.png");

    Player player = Player();
    BlockFactory::getInstance().initBlocks();

    Chunk chunk = Chunk({0, -config::CHUNK_SIZE/2,0}, 0);
    // chunk.generate_default_blocks(config::CHUNK_HEIGHT / 2);
    chunk.generate_perlin(2137u);
    // chunk.generate_mesh();

    World *world = new World(); // stack overfow for bigger worlds if allocated on stack;
    Benchmark bench("Chunk mesh ", 1);
    Benchmark single("Single chunk mesh, with neighbours", 1);
    bench.start();
        world->generate_perlin_chunks(10000000u);
        world->mesh_all_chunks();
    bench.stop(world->side * world->side * world->height);
    single.start();
        int single_iters = 1000;
        for (int i = 0; i < single_iters; i++) {
            world->get_chunk_raw_access({13, 2, 1}).generate_mesh();
        }
    single.stop(single_iters);


    bool DAWG = true; // to skip initial lag
    float startTime = GetTime() + 5.0f; // count from now, just before main loop
    const float deltaPeriod = 0.0f;
    const float stopTime = startTime + 2;
    //==================================== RLGL (opengl abstr) changes ============================================
    rlSetLineWidth(3.0f); // lines are finally more visible and not as annoying
    // rlEnableWireMode(); // Draw wires only!

    // Main loop ==============================
    while (!WindowShouldClose()) {

        // FLUID DEMO ==================================================================================================
        if (GetTime() > startTime && GetTime() < stopTime && DAWG) {
            for (int x = -4; x <= 4; x++) {
                for (int z = 6; z <= 10; z++) {
                    world->fluidSim.addFluid(x, 15, z);
                }
            }
            // world->addFluid(-4, 20, 9);
            startTime += deltaPeriod;
            // DAWG = false;
        }

        // UPDATE WORLD AND SIMULATION =================================================================================
        UpdateCamera(&player.camera, CAMERA_FREE); // movement related
        world->update();

        // Wire debugger
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
                DrawCubeWiresV((Vector3){ 0.0f, 0.5f, 1.0f }, (Vector3){ 1.0f, 1.0f, 1.0f }, RED);
                DrawCubeV((Vector3){ 0.0f, 0.5f, 1.0f }, (Vector3){ 1.0f, 1.0f, 1.0f }, PURPLE);
                DrawCubeWiresV((Vector3){ 0.0f, 0.5f, -1.0f }, (Vector3){ 1.0f, 1.0f, 1.0f }, DARKGREEN);
                DrawCubeV((Vector3) { 0.0f, 0.5f, -1.0f }, (Vector3){ 1.0f, 3.0f, 1.0f }, YELLOW);
                DrawGrid(100, 1.0f);

                world->draw_all(dirt_plank, DRAW_CHUNK_DEBUG_WIRES_MODEL);
                chunk.draw_chunk(dirt_plank, DRAW_CHUNK_DEBUG_WIRES_MODEL);
                // world->chunks[config::MAP_SIDE_IN_CHUNKS/2][config::MAP_HEIGHT_IN_CHUNKS/2][config::MAP_SIDE_IN_CHUNKS/2].draw_chunk(dirt_plank, true);
                // world->chunks[6][4][13].draw_chunk(dirt_plank, DRAW_CHUNK_DEBUG_WIRES_MODEL);

                // DrawBoundingBox(GetMeshBoundingBox(chunk.chunkMesh), BLACK);
            EndMode3D();

             // debug stays
             DrawRectangleGradientV(0, 0, 350, 120,{246, 233, 107, 255}, {56, 127, 57, 255});
             DrawText("This is a raylib window with ImGui!", 10, 10, 20, BLACK);
             DrawText(TextFormat("Fps: %d  |  Frame time: %.2fms", GetFPS(), GetFrameTime()*1000), 10, 30, 20, BLACK);
             DrawText(TextFormat("  Chunk: %s", world->chunk_cord_of_block(player.camera.position).toString().c_str()), 10, 50, 20, BLACK);
             DrawText(TextFormat("Camera Position: [%.2f, %.2f, %.2f]", player.camera.position.x, player.camera.position.y, player.camera.position.z), 10, 70, 20, BLACK);
             DrawText(TextFormat("  Camera target: [%.2f, %.2f, %.2f]", player.camera.target.x, player.camera.target.y, player.camera.target.z), 10, 90, 20, BLACK);

         EndDrawing();
    }



    CloseWindow();
    bench.results();
    single.results();
    world->print_size_report();
    delete world;
    // UnloadTexture(planktxt);
    // UnloadModel(model);


    return 0;
}
