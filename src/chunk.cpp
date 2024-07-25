#include <cstdlib>
#include "chunk.hpp"

Chunk::Chunk() {;}

void Chunk::generate_default_blocks(int airLevel) {
    // Function implementation
}

void Chunk::draw_chunk(Texture &text) {
    Material material = LoadMaterialDefault();
    material.maps[MATERIAL_MAP_DIFFUSE].texture = text;

    Model model = LoadModelFromMesh(chunkMesh);
    model.materials[0] = material;

    // DEBUG: it may need to be uncommented
    // SetModelMeshMaterial(&model, 0, 0);
    // DrawModelEx(model, {0, 0, 0}, (Vector3){0, 0, 0},
    //             0, (Vector3){1, 1, 1}, WHITE);

    DrawModel(model, {0,0,0}, 1, WHITE);

    // UnloadMaterial(material);
    // UnloadModel(model);
}

void Chunk::generate_mesh() {
    // x, y, z (+-)
    Vector3 NORMALS[] = {
        {1, 0, 0},
        {-1, 0, 0},
        {0, 1, 0},
        {0, -1, 0},
        {0, 0, 1},
        {0, 0, -1}
    };

    constexpr int BLOCKS_IN_CHUNK = config::CHUNK_HEIGHT * config::CHUNK_SIZE * config::CHUNK_SIZE;
    constexpr int VALUES_PER_BLOCK = 6 * 2 * 3 * 3;
    constexpr int TEXTURE_VALS_PER_BLOCK = 6 * 2 * 3 * 2;
    // BLOCKS * (faces) * (tri per face) * (verts) * (floats per vertex) * SIZEOF
    float *vertices = (float*) RL_MALLOC(BLOCKS_IN_CHUNK * VALUES_PER_BLOCK * sizeof(float));
    float *texcoords = (float*) RL_MALLOC(BLOCKS_IN_CHUNK * TEXTURE_VALS_PER_BLOCK * sizeof(float));
    float *normals = (float*) RL_MALLOC(BLOCKS_IN_CHUNK * VALUES_PER_BLOCK * sizeof(float));
    unsigned short *indices = (unsigned short *)malloc(BLOCKS_IN_CHUNK * 6 * 6 * sizeof(unsigned short));

    int vertexCount = 0;
    int indexCount = 0;

    for (int x = 0; x < config::CHUNK_SIZE; x++) {
        for (int y = 0; y < config::CHUNK_HEIGHT; y++) {
            for (int z = 0; z < config::CHUNK_SIZE; z++) {
                if (y > config::CHUNK_HEIGHT / 2) continue; // earth level;

                for (int f = 0; f < 6; f++) {
                    vertices[vertexCount * 3 + 0] = x;
                    vertices[vertexCount * 3 + 1] = y;
                    vertices[vertexCount * 3 + 2] = z;

                    texcoords[vertexCount * 2 + 0] = 0;
                    texcoords[vertexCount * 2 + 1] = 0;

                    normals[vertexCount * 3 + 0] = NORMALS[f].x;
                    normals[vertexCount * 3 + 1] = NORMALS[f].y;
                    normals[vertexCount * 3 + 2] = NORMALS[f].z;

                    indices[vertexCount] = vertexCount;
                    vertexCount++;
                }
            }
        }
    }

    chunkMesh = Mesh();

    chunkMesh.vertices = vertices;
    chunkMesh.texcoords = texcoords;
    chunkMesh.normals = normals;
    chunkMesh.indices = indices;

    chunkMesh.triangleCount = vertexCount; // change it
    chunkMesh.vertexCount = vertexCount;
}


// void Chunk::generate_mesh() {
//     // Define cube vertices, texcoords, normals, and indices
//     float vertices[] = {
//         // Front face
//         -0.5f, -0.5f,  0.5f,  // Bottom-left
//          0.5f, -0.5f,  0.5f,  // Bottom-right
//          0.5f,  0.5f,  0.5f,  // Top-right
//         -0.5f,  0.5f,  0.5f,  // Top-left

//         // Back face
//         -0.5f, -0.5f, -0.5f,  // Bottom-left
//          0.5f, -0.5f, -0.5f,  // Bottom-right
//          0.5f,  0.5f, -0.5f,  // Top-right
//         -0.5f,  0.5f, -0.5f,  // Top-left

//         // Top face
//         -0.5f,  0.5f, -0.5f,  // Bottom-left
//          0.5f,  0.5f, -0.5f,  // Bottom-right
//          0.5f,  0.5f,  0.5f,  // Top-right
//         -0.5f,  0.5f,  0.5f,  // Top-left

//         // Bottom face
//         -0.5f, -0.5f, -0.5f,  // Bottom-left
//          0.5f, -0.5f, -0.5f,  // Bottom-right
//          0.5f, -0.5f,  0.5f,  // Top-right
//         -0.5f, -0.5f,  0.5f,  // Top-left

//         // Right face
//          0.5f, -0.5f, -0.5f,  // Bottom-left
//          0.5f,  0.5f, -0.5f,  // Bottom-right
//          0.5f,  0.5f,  0.5f,  // Top-right
//          0.5f, -0.5f,  0.5f,  // Top-left

//         // Left face
//         -0.5f, -0.5f, -0.5f,  // Bottom-left
//         -0.5f,  0.5f, -0.5f,  // Bottom-right
//         -0.5f,  0.5f,  0.5f,  // Top-right
//         -0.5f, -0.5f,  0.5f   // Top-left
//     };

//     float texcoords[] = {
//         // Front face
//         0.0f, 0.0f,  // Bottom-left
//         1.0f, 0.0f,  // Bottom-right
//         1.0f, 1.0f,  // Top-right
//         0.0f, 1.0f,  // Top-left

//         // Back face
//         0.0f, 0.0f,  // Bottom-left
//         1.0f, 0.0f,  // Bottom-right
//         1.0f, 1.0f,  // Top-right
//         0.0f, 1.0f,  // Top-left

//         // Top face
//         0.0f, 0.0f,  // Bottom-left
//         1.0f, 0.0f,  // Bottom-right
//         1.0f, 1.0f,  // Top-right
//         0.0f, 1.0f,  // Top-left

//         // Bottom face
//         0.0f, 0.0f,  // Bottom-left
//         1.0f, 0.0f,  // Bottom-right
//         1.0f, 1.0f,  // Top-right
//         0.0f, 1.0f,  // Top-left

//         // Right face
//         0.0f, 0.0f,  // Bottom-left
//         1.0f, 0.0f,  // Bottom-right
//         1.0f, 1.0f,  // Top-right
//         0.0f, 1.0f,  // Top-left

//         // Left face
//         0.0f, 0.0f,  // Bottom-left
//         1.0f, 0.0f,  // Bottom-right
//         1.0f, 1.0f,  // Top-right
//         0.0f, 1.0f,  // Top-left
//     };

//     float normals[] = {
//         // Front face
//         0.0f, 0.0f, 1.0f,  // Bottom-left
//         0.0f, 0.0f, 1.0f,  // Bottom-right
//         0.0f, 0.0f, 1.0f,  // Top-right
//         0.0f, 0.0f, 1.0f,  // Top-left

//         // Back face
//         0.0f, 0.0f, -1.0f, // Bottom-left
//         0.0f, 0.0f, -1.0f, // Bottom-right
//         0.0f, 0.0f, -1.0f, // Top-right
//         0.0f, 0.0f, -1.0f, // Top-left

//         // Top face
//         0.0f, 1.0f, 0.0f,  // Bottom-left
//         0.0f, 1.0f, 0.0f,  // Bottom-right
//         0.0f, 1.0f, 0.0f,  // Top-right
//         0.0f, 1.0f, 0.0f,  // Top-left

//         // Bottom face
//         0.0f, -1.0f, 0.0f, // Bottom-left
//         0.0f, -1.0f, 0.0f, // Bottom-right
//         0.0f, -1.0f, 0.0f, // Top-right
//         0.0f, -1.0f, 0.0f, // Top-left

//         // Right face
//         1.0f, 0.0f, 0.0f,  // Bottom-left
//         1.0f, 0.0f, 0.0f,  // Bottom-right
//         1.0f, 0.0f, 0.0f,  // Top-right
//         1.0f, 0.0f, 0.0f,  // Top-left

//         // Left face
//         -1.0f, 0.0f, 0.0f, // Bottom-left
//         -1.0f, 0.0f, 0.0f, // Bottom-right
//         -1.0f, 0.0f, 0.0f, // Top-right
//         -1.0f, 0.0f, 0.0f  // Top-left
//     };

//     for (int i = 0; i < 6 * 4 * 3; i++) {
//         normals[i] = 0.0f;
//     }
//     unsigned short indices[] = {
//         0, 1, 2, 2, 3, 0,       // Front face
//         4, 5, 6, 6, 7, 4,       // Back face
//         8, 9, 10, 10, 11, 8,    // Top face
//         12, 13, 14, 14, 15, 12, // Bottom face
//         16, 17, 18, 18, 19, 16, // Right face
//         20, 21, 22, 22, 23, 20  // Left face
//     };

//     // Create the mesh
//     Mesh mesh = { 0 };
//     mesh.vertexCount = 24;
//     mesh.triangleCount = 12;
//     mesh.vertices = vertices;
//     mesh.texcoords = texcoords;
//     mesh.normals = normals;
//     mesh.indices = indices;

//     chunkMesh = mesh;
//     // Upload the mesh to GPU
//     UploadMesh(&chunkMesh, true);
// }