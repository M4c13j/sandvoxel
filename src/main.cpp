#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <string>
#include <map>
#include <assert.h>
#include "player.hpp"
#include "chunk.hpp"
#include "config.hpp"

int main(int argc, char** argv)
{
    // Initialize raylib ======================
    InitWindow(1200, 700, "Raylib + ImGui Example");
    SetTargetFPS(60);
    SetWindowState(FLAG_WINDOW_RESIZABLE ); //| FLAG_BORDERLESS_WINDOWED_MODE);
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT); // Anti-asliasing and V-Sync

    Mesh mesh = GenMeshPlane(10, 10, 2, 2);
    Model model = LoadModelFromMesh(mesh);
    Texture planktxt = LoadTexture("../resources/textures/planks_oak.png"); // NOTE: path relative to build/ :(
    Texture dirt     = LoadTexture("../resources/textures/dirt.png");       // NOTE: path relative to build/ :(

    printf("%d mc = (1); material %d = (1)\n", model.meshCount, model.materialCount);
    model.meshes = (Mesh*) RL_REALLOC(model.meshes, 4 * sizeof(Mesh)); model.meshCount = 4; assert(model.meshes);
    model.meshes[1] = GenMeshPoly(6, 3);
    // model.meshes[2] = GenMeshCube(5, 3, 2);
    model.meshes[2] = GenMeshHeightmap(LoadImage("../resources/textures/dirt.png"),
                            (Vector3){10,10,10});
    // model.meshes[3] = GenMeshSphere(2, 10, 3);

    Material materialDefault = LoadMaterialDefault();
    materialDefault.maps[MATERIAL_MAP_DIFFUSE].texture = planktxt;
    model.materials = (Material*) RL_REALLOC(model.materials, 3 * sizeof(Material)); model.materialCount = 3; assert(model.materials);
    model.materials[0] = LoadMaterialDefault(); model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = planktxt;
    model.materials[1] = LoadMaterialDefault(); model.materials[1].maps[MATERIAL_MAP_DIFFUSE].texture = dirt;
    model.materials[2] = LoadMaterialDefault();

    assert(model.materialCount == 3 && model.meshCount == 4);

    SetModelMeshMaterial(&model, 0, 0);

    Texture dirt_plank = LoadTexture("../resources/textures/dirt_plank.png");
    Player player = Player();

    Chunk chunk = Chunk({0, -config::CHUNK_HEIGHT/2,0}, 0);
    // chunk.generate_default_blocks(config::CHUNK_HEIGHT / 2);
    chunk.generate_perlin();
    chunk.update_visibility();
    chunk.generate_mesh();

    Block bb = Block(Block::DirtPlank, 0,0);
    // printf("DEBUGGER\n"); return 0;
    // Renderer renderer = Renderer();
    // Main loop ==============================
    while (!WindowShouldClose())
    {
        UpdateCamera(&player.camera, CAMERA_FREE);
        // Rendering
        BeginDrawing();
        ClearBackground(RAYWHITE);

        chunk.generate_mesh();

        BeginMode3D(player.camera);
            DrawCubeWiresV((Vector3){ 0.0f, 0.5f, 1.0f }, (Vector3){ 1.0f, 1.0f, 1.0f }, RED);
            DrawCubeV((Vector3){ 0.0f, 0.5f, 1.0f }, (Vector3){ 1.0f, 1.0f, 1.0f }, PURPLE);
            DrawCubeWiresV((Vector3){ 0.0f, 0.5f, -1.0f }, (Vector3){ 1.0f, 1.0f, 1.0f }, DARKGREEN);
            DrawCubeV((Vector3) { 0.0f, 0.5f, -1.0f }, (Vector3){ 1.0f, 3.0f, 1.0f }, YELLOW);
            DrawGrid(100, 1.0f);

            // DrawModel(model, (Vector3){.0f,.0f,.0f}, 1.0f, WHITE);

            chunk.draw_chunk(dirt_plank);
            // bb.draw_face({-10, 10, 10}, DIR_UP);

            // for (int dir = 0; dir < COUNT_DIR; dir++) {
            //     bb.draw_face({-2*dir, 2, 0}, static_cast<Dir>(dir));
            // }
            DrawBoundingBox(GetMeshBoundingBox(chunk.chunkMesh), BLACK);
         EndMode3D();

         // debug stays
         DrawText("This is a raylib window with ImGui!", 10, 10, 20, DARKGRAY);
         DrawText(TextFormat("Fps: %d", GetFPS()), 10, 30, 20, DARKGRAY);
         DrawText(TextFormat("Camera Position: [%.2f, %.2f, %.2f]", player.camera.position.x, player.camera.position.y, player.camera.position.z), 10, 50, 20, DARKGRAY);
         DrawText(TextFormat("  Camera target: [%.2f, %.2f, %.2f]", player.camera.target.x, player.camera.target.y, player.camera.target.z), 10, 70, 20, DARKGRAY);
         DrawText(TextFormat("Mesh Count: %d", model.meshCount), 10, 90, 20, DARKGRAY);
         DrawText(TextFormat("Material Count: %d", model.materialCount), 10, 110, 20, DARKGRAY);

         // Rendering
         EndDrawing();
    }

    CloseWindow();
    UnloadTexture(planktxt);
    UnloadModel(model);

    return 0;
}
