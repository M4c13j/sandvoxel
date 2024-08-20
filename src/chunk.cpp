#include <cstdlib>
#include <cassert>
#include <algorithm>
#include <cstdio>

#include "raylib.h"
#include "raymath.h"
#include "chunk.hpp"
#include "block.hpp"
#include "config.hpp"
#include "perlin.hpp"

void Chunk::generate_default_blocks(int airLevel) {
    for (int x = 0; x < config::CHUNK_SIZE; x++) {
        for (int y = 0; y < config::CHUNK_HEIGHT; y++) {
            for (int z = 0; z < config::CHUNK_SIZE; z++) {
                if (y< airLevel) {
                    block[x][y][z] = Block(Block::DirtPlank, 0, 0, {x,y,z});
                } else {
                    block[x][y][z] = Block(Block::Air, 0.5, 0.5, {x,y,z});
                }
            }
        }
    }
}

void Chunk::generate_perlin(uint_fast32_t seed) {
    const siv::PerlinNoise::seed_type seedG = seed;//(uint_fast16_t) GetMousePosition().x;
    const siv::PerlinNoise perlin{seedG};

    for (int x = 0; x < config::CHUNK_SIZE; x++) {
        for (int z = 0; z < config::CHUNK_SIZE; z++) {
            int glevel = perlin.noise2D_01((x+cords.x*config::CHUNK_SIZE)*0.1, (z+cords.z*config::CHUNK_SIZE)*0.1) * config::CHUNK_HEIGHT;
            for (int y = 0; y < config::CHUNK_HEIGHT; y++) {
                if (y < glevel) {
                    block[x][y][z] = Block(Block::DirtPlank, 0, 0, {x,y,z});
                } else {
                    block[x][y][z] = Block(Block::Air, 0.5, 0.5, {x,y,z});
                }
            }
        }
    }
}

void Chunk::draw_chunk(Texture &text) {
    // Material material = LoadMaterialDefault();
    // material.maps[MATERIAL_MAP_DIFFUSE].texture = text;

    // Model model = LoadModelFromMesh(chunkMesh);
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = text;

    // DEBUG: it may need to be uncommented
    // SetModelMeshMaterial(&model, 0, 0);
    // DrawModelEx(model, {0, 0, 0}, (Vector3){0, 0, 0},
    //             0, (Vector3){1, 1, 1}, WHITE);

    DrawModel(model, {0,0,0}, 1, WHITE);

    // UnloadMaterial(material);
    // UnloadModel(model);
}

inline bool is_in_chunk(Cord pos) {
    return (pos.x < config::CHUNK_SIZE && pos.x >= 0) &&
        (pos.y < config::CHUNK_HEIGHT && pos.y >= 0) &&
        (pos.z < config::CHUNK_SIZE && pos.z >= 0);
}

// inline bool is_visible_dir(const World &world, Dir dir) {
//     if (is_in_chunk())
// }

int Chunk::set_visible_faces() {
    int visCount = 0;
    for (int x = 0; x < config::CHUNK_SIZE; x++) {
        for (int y = 0; y < config::CHUNK_HEIGHT; y++) {
            for (int z = 0; z < config::CHUNK_SIZE; z++) {
                Block *curr = &block[x][y][z];
                curr->visible.reset();
                if (curr->type == Block::Air) {
                    continue; // do not draw Air
                }

                Cord pos{x,y,z};
                for (int dir = 0; dir < COUNT_DIR; dir++) {
                    Cord nextPos = pos + FACE_NORMALS[dir];
                    if (is_in_chunk(nextPos) &&  get_block(nextPos)->type == Block::Air) {
                        curr->visible[dir] = true;
                        visCount++;
                    }
                }
            }
        }
    }
    return visCount;
}

//temoprarily disabled
void Chunk::update_visibility() {
    printf("GAYASS\n\n\n");
    inverse_dir(DIR_DOWN);
}

void Chunk::generate_mesh() {
    const int FACES_TO_DRAW = set_visible_faces();
    // constexpr int BLOCKS_IN_CHUNK = config::CHUNK_HEIGHT * config::CHUNK_SIZE * config::CHUNK_SIZE;
    const int VERTEX_DATA_TOTAL = VERTEX_DATA_PER_FACE * FACES_TO_DRAW;
    const int TEXTURE_DATA_TOTAL = TEXTURE_DATA_PER_FACE * FACES_TO_DRAW;
    const int INDICES_DATA_TOTAL = INDICES_DATA_PER_FACE * FACES_TO_DRAW;

    float *vertices = (float*) RL_MALLOC(VERTEX_DATA_TOTAL * sizeof(float));
    float *texcoords = (float*) RL_MALLOC(TEXTURE_DATA_TOTAL * sizeof(float));
    float *normals = (float*) RL_MALLOC(VERTEX_DATA_TOTAL * sizeof(float));
    unsigned short *indices = (unsigned short *)RL_MALLOC(INDICES_DATA_TOTAL * sizeof(unsigned short));

    FacePlacementData placementData = {vertices, texcoords, normals, indices, 0};
    int vertexCount = 0;
    int indexCount = 0;

    // wish I could foreach or have flexibility from java to use queue as array.
    for (int x = 0; x < config::CHUNK_SIZE; x++) {
        for (int y = 0; y < config::CHUNK_HEIGHT; y++) {
            for (int z = 0; z < config::CHUNK_SIZE; z++) {
                Block *curr = &block[x][y][z];
                for (int dir = 0; dir < COUNT_DIR; dir++) {
                    if (curr->visible[dir]) {
                        Vector3 currBlockPos = Vector3Add(drawPos, {x,y,z});
                        curr->generate_face(placementData, static_cast<Dir>(dir), currBlockPos);
                        placementData.advance_face();
                        vertexCount += 4;
                        indexCount += 2;
                    }
                }
            }
        }
    }

    // UnloadMesh(chunkMesh);
    chunkMesh = { 0 };

    chunkMesh.vertices = vertices;
    chunkMesh.texcoords = texcoords;
    chunkMesh.normals = normals;
    chunkMesh.indices = indices;

    chunkMesh.triangleCount = indexCount; // change it
    chunkMesh.vertexCount = vertexCount;

    UploadMesh(&chunkMesh, false);
    model = LoadModelFromMesh(chunkMesh);;
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


