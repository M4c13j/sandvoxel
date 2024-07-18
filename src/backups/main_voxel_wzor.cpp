#include "raylib.h"
#include <stdlib.h>
#include <stdbool.h>
#include "raymath.h"
#define CHUNK_SIZE 16
#define MAP_WIDTH 4
#define MAP_HEIGHT 1
#define MAP_DEPTH 4

#define BLOCK_TYPE_COUNT 2
#define BLOCK_FACE_COUNT 6

// Voxel map storage for chunks
unsigned char voxelChunks[MAP_WIDTH][MAP_HEIGHT][MAP_DEPTH][CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
Mesh chunkMeshes[MAP_WIDTH][MAP_HEIGHT][MAP_DEPTH];

// Define the UV coordinates for each block type and face
Vector2 blockUVs[BLOCK_TYPE_COUNT][BLOCK_FACE_COUNT][4];

// Function to check if a voxel face is visible
bool IsVoxelFaceVisible(int x, int y, int z, int dx, int dy, int dz, int chunkX, int chunkY, int chunkZ) {
    int nx = x + dx;
    int ny = y + dy;
    int nz = z + dz;

    if (nx < 0 || nx >= CHUNK_SIZE || ny < 0 || ny >= CHUNK_SIZE || nz < 0 || nz >= CHUNK_SIZE) {
        return true;
    }

    return voxelChunks[chunkX][chunkY][chunkZ][nx][ny][nz] == 0;
}

// Function to generate voxel chunk (simple example)
void GenerateVoxelChunk(int chunkX, int chunkY, int chunkZ) {
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                if (y < CHUNK_SIZE / 2) voxelChunks[chunkX][chunkY][chunkZ][x][y][z] = 1; // Ground blocks
                else voxelChunks[chunkX][chunkY][chunkZ][x][y][z] = 0; // Air blocks
            }
        }
    }
}

// Function to create a mesh for a voxel chunk
Mesh GenerateVoxelMesh(int chunkX, int chunkY, int chunkZ) {
    float *vertices = (float *)malloc(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 6 * 6 * 3 * sizeof(float));
    float *texcoords = (float *)malloc(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 6 * 6 * 2 * sizeof(float));
    float *normals = (float *)malloc(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 6 * 6 * 3 * sizeof(float));
    unsigned short *indices = (unsigned short *)malloc(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 6 * 6 * sizeof(unsigned short));

    int vertexCount = 0;
    int indexCount = 0;

     for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                int blockType = voxelChunks[chunkX][chunkY][chunkZ][x][y][z];
                if (blockType == 0) continue;

                // ... (existing code) ...

                for (int f = 0; f < 6; f++) {
                    if (!IsVoxelFaceVisible(x, y, z, normalsVector[f].x, normalsVector[f].y, normalsVector[f].z, chunkX, chunkY, chunkZ)) continue;

                    for (int i = 0; i < 6; i++) {
                        int vertexIndex = faceIndices[f][i];
                        vertices[vertexCount * 3] = cubeVertices[vertexIndex].x;
                        vertices[vertexCount * 3 + 1] = cubeVertices[vertexIndex].y;
                        vertices[vertexCount * 3 + 2] = cubeVertices[vertexIndex].z;

                        // Map UV coordinates correctly
                        int uvIndex = i % 4;
                        texcoords[vertexCount * 2] = blockUVs[blockType - 1][f][uvIndex].x;
                        texcoords[vertexCount * 2 + 1] = blockUVs[blockType - 1][f][uvIndex].y;

                        normals[vertexCount * 3] = normalsVector[f].x;
                        normals[vertexCount * 3 + 1] = normalsVector[f].y;
                        normals[vertexCount * 3 + 2] = normalsVector[f].z;

                        indices[indexCount] = vertexCount;

                        vertexCount++;
                        indexCount++;
                    }
                }
            }
        }
    }

    Mesh mesh = { 0 };
    mesh.vertexCount = vertexCount;
    mesh.triangleCount = indexCount / 3;

    mesh.vertices = vertices;
    mesh.texcoords = texcoords;
    mesh.normals = normals;
    mesh.indices = indices;

    UploadMesh(&mesh, true);

    return mesh;
}

// Generate meshes for all chunks
void GenerateChunkMeshes() {
    for (int x = 0; x < MAP_WIDTH; x++) {
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int z = 0; z < MAP_DEPTH; z++) {
                GenerateVoxelChunk(x, y, z);
                chunkMeshes[x][y][z] = GenerateVoxelMesh(x, y, z);
            }
        }
    }
}

// Initialize UV coordinates for blocks
void InitializeBlockUVs(Texture2D atlas) {
    float texWidth = (float)atlas.width;
    float texHeight = (float)atlas.height;
    float blockWidth = 1.0f / 16.0f; // Assuming 16x16 blocks in the atlas

    // Define UV coordinates for each block type and face
    for (int blockType = 0; blockType < BLOCK_TYPE_COUNT; blockType++) {
        for (int face = 0; face < BLOCK_FACE_COUNT; face++) {
            int texX = blockType % 16;
            int texY = blockType / 16;

            blockUVs[blockType][face][0] = (Vector2){ texX * blockWidth, texY * blockWidth };
            blockUVs[blockType][face][1] = (Vector2){ (texX + 1) * blockWidth, texY * blockWidth };
            blockUVs[blockType][face][2] = (Vector2){ (texX + 1) * blockWidth, (texY + 1) * blockWidth };
            blockUVs[blockType][face][3] = (Vector2){ texX * blockWidth, (texY + 1) * blockWidth };
        }
    }
}

int main(int argc, char **argv) {
    // Initialize raylib ======================
    InitWindow(1200, 700, "Raylib Voxel Map Example");
    SetTargetFPS(60);

    Camera camera = {
        .position = (Vector3){20.0f, 20.0f, 20.0f},
        .target = (Vector3){0.0f, 0.0f, 0.0f},
        .up = (Vector3){0.0f, 1.0f, 0.0f},
        .fovy = 60.0f,
        .projection = CAMERA_PERSPECTIVE
    };

    // Load texture atlas
    Texture2D atlas = LoadTexture("../resources/textures/dirt.png");

    // Initialize block UV coordinates
    InitializeBlockUVs(atlas);

    // Generate the voxel map
    GenerateChunkMeshes();

    Material material = LoadMaterialDefault();
    SetMaterialTexture(&material, MATERIAL_MAP_DIFFUSE, atlas);
    // Main loop ==============================
    while (!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_FREE);

        // Rendering
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        for (int x = 0; x < MAP_WIDTH; x++) {
            for (int y = 0; y < MAP_HEIGHT; y++) {
                for (int z = 0; z < MAP_DEPTH; z++) {
                    DrawMesh(chunkMeshes[x][y][z], material, MatrixIdentity());
                }
            }
        }

        EndMode3D();

        DrawText("Voxel Map Example with Textures", 10, 10, 20, DARKGRAY);
        DrawTexture(atlas, 100, 100, WHITE);
        EndDrawing();
    }

    // De-Initialization ======================
    for (int x = 0; x < MAP_WIDTH; x++) {
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int z = 0; z < MAP_DEPTH; z++) {
                UnloadMesh(chunkMeshes[x][y][z]);
            }
        }
    }

    UnloadTexture(atlas);

    CloseWindow();

    return 0;
}
