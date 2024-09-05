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


    Texture dirt_plank = LoadTexture("../resources/textures/dirt_plank.png");

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


    bool DAWG = true; // to skip initial lag
    float startTimefl = GetTime() + 5.0f; // count from now, just before main loop
    float startTimesa = GetTime() + 2.0f;
    float stopTimesa = GetTime() + 100;
    const float stopTimefl = startTimefl + 2;
    //==================================== RLGL (opengl abstr) changes ============================================
    rlSetLineWidth(3.0f); // lines are finally more visible and not as annoying
    // rlEnableWireMode(); // Draw wires only!

    // Main loop ==============================
    while (!WindowShouldClose()) {

        // FLUID DEMO ==================================================================================================
        // if (GetTime() > startTimefl && GetTime() < stopTimefl && DAWG) {
        if (IsKeyDown(KEY_N)) {
            for (int x = -4; x <= 4; x++) {
                for (int z = 6; z <= 10; z++) {
                    world->fluidSim.addBlock(x-23, 47, z-21);
                }
            }
            startTimefl += 0.1;
            // DAWG = false;
        }

        // SAND DEMO =================================================
        // if (GetTime() > startTimesa && GetTime() < stopTimesa && DAWG) {
        if (IsKeyDown(KEY_B)) {
            for (int x = -2; x <= 1; x++) {
                for (int z = 6; z <= 8; z++) {
                    world->sandSim.addBlock(x - 10, 15 + 30, z - 10);
                    world->sandSim.addBlock(x - 10, 15 - 30 + 1, z - 10);
                }
            }
            world->sandSim.addBlock(-15 - 10, 30, -15 - 10);
            startTimesa += 0.1f;
        }

        // BELOW PLAYER ================================================
        Cord currPos{static_cast<int>(player.camera.position.x), static_cast<int>(player.camera.position.y),
                  static_cast<int>(player.camera.position.z)};

        if (IsKeyDown(KEY_V)) {
            world->sandSim.addBlock(currPos);
        }
        if (IsKeyDown(KEY_M)) {
            world->fluidSim.addBlock(currPos);
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
                // DrawCubeWiresV((Vector3){ 0.0f, 0.5f, 1.0f }, (Vector3){ 1.0f, 1.0f, 1.0f }, RED);
                // DrawCubeV((Vector3){ 0.0f, 0.5f, 1.0f }, (Vector3){ 1.0f, 1.0f, 1.0f }, PURPLE);
                // DrawCubeWiresV((Vector3){ 0.0f, 0.5f, -1.0f }, (Vector3){ 1.0f, 1.0f, 1.0f }, DARKGREEN);
                // DrawCubeV((Vector3) { 0.0f, 0.5f, -1.0f }, (Vector3){ 1.0f, 3.0f, 1.0f }, YELLOW);
                // DrawGrid(100, 1.0f);

                world->draw_all(dirt_plank, DRAW_CHUNK_DEBUG_WIRES_MODEL);
                chunk.draw_chunk(dirt_plank, DRAW_CHUNK_DEBUG_WIRES_MODEL);
                // world->chunks[config::MAP_SIDE_IN_CHUNKS/2][config::MAP_HEIGHT_IN_CHUNKS/2][config::MAP_SIDE_IN_CHUNKS/2].draw_chunk(dirt_plank, true);
                // world->chunks[6][4][13].draw_chunk(dirt_plank, DRAW_CHUNK_DEBUG_WIRES_MODEL);

                // DrawBoundingBox(GetMeshBoundingBox(chunk.chunkMesh), BLACK);
            EndMode3D();

             // debug status====================================================================================================
             DrawRectangleGradientV(0, 0, 350, 120,BLUE, RED);
             DrawText("SandVoxel debug.", 10, 10, 20, BLACK);
             DrawText(TextFormat("Fps: %d  |  Frame time: %.2fms", GetFPS(), GetFrameTime()*1000), 10, 30, 20, BLACK);
             DrawText(TextFormat("Chunk: %s", world->chunk_cord_of_block(player.camera.position).toString().c_str()), 10, 50, 20, BLACK);
             DrawText(TextFormat("pos: [%.2f, %.2f, %.2f]", player.camera.position.x, player.camera.position.y, player.camera.position.z), 10, 70, 20, BLACK);
             DrawText(TextFormat("target: [%.2f, %.2f, %.2f]", player.camera.target.x, player.camera.target.y, player.camera.target.z), 10, 90, 20, BLACK);

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
