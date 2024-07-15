#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <string>
#include <map>
#include <assert.h>

int main(int argc, char** argv)
{
    // Initialize raylib ======================
    InitWindow(1200, 700, "Raylib + ImGui Example");
    SetTargetFPS(60);
    SetWindowState(FLAG_WINDOW_RESIZABLE ); //| FLAG_BORDERLESS_WINDOWED_MODE);
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);

    Camera camera = {
        .position = (Vector3){ 2.0f, 2.0f, 3.0f },    // Camera position
        .target = (Vector3){ 0.0f, 0.5f, 0.0f },      // Camera looking at point
        .up = (Vector3){ 0.0f, 1.0f, 0.0f },          // Camera up vector (rotation towards target)
        .fovy = 60.0f,                                // Camera field-of-view Y
        .projection = CAMERA_PERSPECTIVE              // Camera projection type
    };
    Mesh mesh = GenMeshPlane(10, 10, 2, 2);
    // ExportMeshAsCode(mesh, "mesh_code.txt");
    ExportMesh(mesh, "./mesh_norm.obj");
    Model model = LoadModelFromMesh(mesh);
    Texture planktxt = LoadTexture("../resources/textures/planks_oak.png"); // NOTE: path relative to build/ :(
    Texture dirt = LoadTexture("../resources/textures/dirt.png"); // NOTE: path relative to build/ :(

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

    printf("%d (3) material %d (3) mesh", model.materialCount, model.meshCount);
    assert(model.materialCount == 3 && model.meshCount == 4);

    SetModelMeshMaterial(&model, 0, 0);
    SetModelMeshMaterial(&model, 1, 0);
    SetModelMeshMaterial(&model, 2, 1);
    SetModelMeshMaterial(&model, 3, 1);
    printf("Jeszcze działa!\n");

    // Define transformation matrices for each mesh
    Matrix transforms[4] =  { 0.0f, 0.0f, 0.0f,
                              5.0f, 0.0f, 0.0f,
                             -5.0f, 0.0f, 0.0f,
                              0.0f, 5.0f, 0.0f};

    // Main loop ==============================
    while (!WindowShouldClose())
    {
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);
        // Rendering
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
            DrawCubeWiresV((Vector3){ 0.0f, 0.5f, 1.0f }, (Vector3){ 1.0f, 1.0f, 1.0f }, RED);
            DrawCubeV((Vector3){ 0.0f, 0.5f, 1.0f }, (Vector3){ 1.0f, 1.0f, 1.0f }, PURPLE);
            DrawCubeWiresV((Vector3){ 0.0f, 0.5f, -1.0f }, (Vector3){ 1.0f, 1.0f, 1.0f }, DARKGREEN);
            DrawCubeV((Vector3) { 0.0f, 0.5f, -1.0f }, (Vector3){ 1.0f, 3.0f, 1.0f }, YELLOW);
            DrawGrid(100, 1.0f);

            DrawModel(model, (Vector3){.0f,.0f,.0f}, 1.0f, WHITE);

         EndMode3D();

        DrawText("This is a raylib window with ImGui!", 10, 10, 20, DARKGRAY);
        DrawText(TextFormat("Fps: %d", GetFPS()), 10, 30, 20, DARKGRAY);
        DrawText(TextFormat("Camera Position: [%.2f, %.2f, %.2f]", camera.position.x, camera.position.y, camera.position.z), 10, 50, 20, DARKGRAY);
        DrawText(TextFormat("  Camera target: [%.2f, %.2f, %.2f]", camera.target.x, camera.target.y, camera.target.z), 10, 70, 20, DARKGRAY);
        DrawText(TextFormat("Mesh Count: %d", model.meshCount), 10, 90, 20, DARKGRAY);
        DrawText(TextFormat("Material Count: %d", model.materialCount), 10, 110, 20, DARKGRAY);

        // Rendering
        EndDrawing();
    }
    std::string dawg;

    CloseWindow();
    UnloadTexture(planktxt);
    UnloadModel(model);

    return 0;
}
