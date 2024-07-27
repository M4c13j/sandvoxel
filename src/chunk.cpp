#include <cstdlib>
#include <cassert>
#include <algorithm>
#include <cstdio>

#include "chunk.hpp"
#include "block.hpp"
#include "config.hpp"
#include "raylib.h"
#include "raymath.h"

Chunk::Chunk() {;}

void Chunk::generate_default_blocks(int airLevel) {
    for (int x = 0; x < config::CHUNK_SIZE; x++) {
        for (int y = 0; y < config::CHUNK_HEIGHT; y++) {
            for (int z = 0; z < config::CHUNK_SIZE; z++) {
                // if (y > airLevel) continue; // earth level;
                bool off = x+y+z % 2 == 0;
               block[x][y][z] = Block(0, 0, {x,y,z});
            }
        }
    }
}

void Chunk::draw_chunk(Texture &text) {
    Material material = LoadMaterialDefault();
    material.maps[MATERIAL_MAP_DIFFUSE].texture = text;

    Model model = LoadModelFromMesh(chunkMesh);
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = text;

    // DEBUG: it may need to be uncommented
    // SetModelMeshMaterial(&model, 0, 0);
    // DrawModelEx(model, {0, 0, 0}, (Vector3){0, 0, 0},
    //             0, (Vector3){1, 1, 1}, WHITE);

    DrawModel(model, {0,0,0}, 1, WHITE);

    // UnloadMaterial(material);
    // UnloadModel(model);
}

void Chunk::generate_mesh() {
    constexpr int BLOCKS_IN_CHUNK = config::CHUNK_HEIGHT * config::CHUNK_SIZE * config::CHUNK_SIZE;
    constexpr int VALUES_PER_BLOCK = 24 * 3;
    constexpr int TEXTURE_VALS_PER_BLOCK = 24 * 2;
    constexpr int INDICES_PER_BLOCK = 6 * 2 * 3;

    float *vertices = (float*) RL_MALLOC(BLOCKS_IN_CHUNK * VALUES_PER_BLOCK * sizeof(float));
    float *texcoords = (float*) RL_MALLOC(BLOCKS_IN_CHUNK * TEXTURE_VALS_PER_BLOCK * sizeof(float));
    float *normals = (float*) RL_MALLOC(BLOCKS_IN_CHUNK * VALUES_PER_BLOCK * sizeof(float));
    unsigned short *indices = (unsigned short *)malloc(BLOCKS_IN_CHUNK * INDICES_PER_BLOCK  * sizeof(unsigned short));

    int vertexCount = 0;
    int indexCount = 0;

    cords = {0,0,0}; // DEBUG
    for (int x = 0; x < config::CHUNK_SIZE; x++) {
        for (int y = 0; y < config::CHUNK_HEIGHT; y++) {
            for (int z = 0; z < config::CHUNK_SIZE; z++) {
                Vector3 startingPos = Vector3Add((Vector3){x,y,z}, cords); // goofy ass
                gen_mesh_block(vertices + 3*vertexCount, texcoords + 2*vertexCount, normals + 3*vertexCount,
                    indices + 3*indexCount, block[x][y][z], startingPos, vertexCount);

                printf("Cube gen: %d %d %d ", (int)startingPos.x,  (int)startingPos.y, (int)startingPos.z);
                printf(" pos: %d %d %d. \n", x, y, z);
                vertexCount += 24;
                indexCount += 12;
            }
        }
    }

    chunkMesh = { 0 };

    // free(chunkMesh.vertices);
    // free(chunkMesh.texcoords);
    // free(chunkMesh.normals);
    // free(chunkMesh.indices);

    chunkMesh.vertices = vertices;
    chunkMesh.texcoords = texcoords;
    chunkMesh.normals = normals;
    chunkMesh.indices = indices;

    chunkMesh.triangleCount = indexCount; // change it
    chunkMesh.vertexCount = vertexCount;

    UploadMesh(&chunkMesh, false);
}

void Chunk::gen_mesh_block(float *vertPt, float *texPt, float *normalPt,
        unsigned short *indiPt, Block &block, Vector3 pos, int index) {

    static Mesh cube_temp = GenMeshCube(1, 1, 1);
    // Mesh cube_temp = GenMeshCube(1, 1, 1);
    int vertexCount = cube_temp.vertexCount;
    assert(vertexCount == 24);

    // std::copy_n(cube_temp->vertices, 3 * vertexCount, vertPt); // 24 * 3
    std::copy_n(cube_temp.normals, 3 * vertexCount, normalPt); // 24 * 3
    // std::copy_n(cube_temp->texcoords, 2 * vertexCount, normalPt); // 24 * 2

    // GenMesh generates cube with middle at 0, so +0.5 corrects it;
    for (int v = 0; v < vertexCount; v++) {
        vertPt[3 * v + 0] = cube_temp.vertices[3 * v + 0] + pos.x + 0.5;
        vertPt[3 * v + 1] = cube_temp.vertices[3 * v + 1] + pos.y + 0.5;
        vertPt[3 * v + 2] = cube_temp.vertices[3 * v + 2] + pos.z + 0.5;
        texPt[2*v + 0] = cube_temp.texcoords[2*v + 0]; //+ block.tx;
        texPt[2*v + 1] = cube_temp.texcoords[2*v + 1]; //block.ty;
        assert(texPt[2*v] <= 1.0 && texPt[2*v + 1] <= 1.0);
        assert(texPt[2*v] >= 0 && texPt[2*v + 1] >= 0);
    }

    std::copy_n(cube_temp.indices, cube_temp.triangleCount * 3, indiPt); // 6 * 3 * 2
    std::transform(indiPt, indiPt + cube_temp.triangleCount * 3, indiPt, [index](int val) {
        return val + index;
    });
}


