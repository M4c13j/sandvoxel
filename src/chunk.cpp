#include <cstdlib>
#include <cassert>
#include <algorithm>
#include <cstdio>

#include "block.hpp"
#include "chunk.hpp"
#include "config.hpp"
#include "perlin.hpp"
#include "raylib.h"
#include "raymath.h"

Chunk::~Chunk() {
    // UnloadModel(model);
    // UnloadMesh(chunkMesh);
}

void Chunk::generate_default_blocks(int airLevel) {
    for (int x = 0; x < config::CHUNK_SIZE; x++) {
        for (int y = 0; y < config::CHUNK_SIZE; y++) {
            for (int z = 0; z < config::CHUNK_SIZE; z++) {
                if (y< airLevel)
                    blocks[x][y][z] = Block(Block::DirtPlank);
                else
                    blocks[x][y][z] = Block(Block::Air);

            }
        }
    }
}

void Chunk::generate_perlin(uint_fast32_t seed) {
    const siv::PerlinNoise::seed_type seedG = seed;//(uint_fast16_t) GetMousePosition().x;
    const siv::PerlinNoise perlin{seedG};
    const float PRECISION_FOR_PERLIN = 0.1;
    for (int x = 0; x < config::CHUNK_SIZE; x++) {
        for (int z = 0; z < config::CHUNK_SIZE; z++) {
            int glevel = perlin.noise2D_01(
                (x+cords.x*config::CHUNK_SIZE)*PRECISION_FOR_PERLIN,
                (z+cords.z*config::CHUNK_SIZE)*PRECISION_FOR_PERLIN
                ) * config::CHUNK_SIZE;
            nonEmptyBlocks += glevel;
            for (int y = 0; y < config::CHUNK_SIZE; y++) {
                if (y < glevel) {
                    blocks[x][y][z] = Block(Block::DirtPlank);
                } else {
                    blocks[x][y][z] = Block(Block::Air);
                }
            }
        }
    }
}

void Chunk::draw_chunk(Texture &text, bool drawBoundingBox) {
    if (!isVisible())
        return; // empty chunk, don't waste time
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = text;

    DrawModel(model, {0,0,0}, 1, WHITE);
    if(drawBoundingBox) {
        DrawBoundingBox(boundingBox, BLACK);
        Vector3 cubeDrawPos = Vector3Add(drawPos, Vector3Scale({1,1,1}, config::CHUNK_SIZE/2));
        DrawCubeWires(cubeDrawPos, config::CHUNK_SIZE, config::CHUNK_SIZE, config::CHUNK_SIZE, BLUE);
    }

    // UnloadMaterial(material);
    // UnloadModel(model);
}

const Cord FACE_NORMALS_CORD[] = { // temporary TODO: remove it
    {0, 0, 1},
    {0, 0,-1},
    {0, 1, 0},
    {0,-1, 0},
    {1, 0, 0},
    {-1, 0, 0}
};
// Returns visibilit of block at dir from pos. CHecks neighbouring chunk if needed.
bool Chunk::is_visible_face(Cord pos, Dir dir) {
    Cord nextPos
        = {pos.x + FACE_NORMALS_CORD[dir].x, pos.y + FACE_NORMALS_CORD[dir].y, pos.z + FACE_NORMALS_CORD[dir].z};
    if (is_in_chunk(nextPos)) {
        return get_block(nextPos.x, nextPos.y, nextPos.z).is_transparent();
    }

    if (neighbours[dir] == nullptr)
        return false;

    switch (dir) {
        case DIR_NORTH: nextPos.z = 0; break;
        case DIR_SOUTH: nextPos.z = config::CHUNK_SIZE - 1; break;
        case DIR_UP: nextPos.y = 0; break;
        case DIR_DOWN: nextPos.y = config::CHUNK_SIZE - 1; break;
        case DIR_EAST: nextPos.x = 0; break;
        case DIR_WEST: nextPos.x = config::CHUNK_SIZE - 1; break;
        }

    return neighbours[dir]->get_block(nextPos.x, nextPos.y, nextPos.z).is_transparent();
}

int Chunk::check_visible_faces() {
    int visCount = 0;
    for (int x = 0; x < config::CHUNK_SIZE; x++) {
        for (int y = 0; y < config::CHUNK_SIZE; y++) {
            for (int z = 0; z < config::CHUNK_SIZE; z++) {
                Block &curr  = blocks[x][y][z];
                curr.visible = 0;
                if (curr.is_transparent())
                    continue; // do not draw Air

                Cord pos{x, y, z};
                for (int dir = 0; dir < DIR_COUNT; dir++) {
                    if (is_visible_face(pos, static_cast<Dir>(dir))) {
                        curr.visible |= 1 << dir;
                        visCount++;
                    }
                }
            }
        }
    }
    return visCount;
}

void Chunk::generate_mesh() {
    if (isEmpty())
        return; // why bother?
    visibleFaces = check_visible_faces();
    // constexpr int BLOCKS_IN_CHUNK = config::CHUNK_HEIGHT * config::CHUNK_SIZE * config::CHUNK_SIZE;
    const int VERTEX_DATA_TOTAL = VERTEX_DATA_PER_FACE * visibleFaces;
    const int TEXTURE_DATA_TOTAL = TEXTURE_DATA_PER_FACE * visibleFaces;
    const int INDICES_DATA_TOTAL = INDICES_DATA_PER_FACE * visibleFaces;

    float *vertices = (float*) RL_MALLOC(VERTEX_DATA_TOTAL * sizeof(float));
    float *texcoords = (float*) RL_MALLOC(TEXTURE_DATA_TOTAL * sizeof(float));
    float *normals = (float*) RL_MALLOC(VERTEX_DATA_TOTAL * sizeof(float));
    unsigned short *indices = (unsigned short *)RL_MALLOC(INDICES_DATA_TOTAL * sizeof(unsigned short));

    FacePlacementData placementData = {vertices, texcoords, normals, indices, 0};
    int vertexCount = 0;
    int indexCount = 0;

    // wish I could foreach or have flexibility from java to use queue as array.
    for (int x = 0; x < config::CHUNK_SIZE; x++) {
        for (int y = 0; y < config::CHUNK_SIZE; y++) {
            for (int z = 0; z < config::CHUNK_SIZE; z++) {
                Block *curr = &blocks[x][y][z];
                for (int dir = 0; dir < DIR_COUNT; dir++) {
                    if (curr->visible & (1<<dir)) {
                        Vector3 currBlockPos = Vector3Add(
                            drawPos, {static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)});
                        curr->generate_face(placementData, static_cast<Dir>(dir), currBlockPos);
                        placementData.advance_face();
                        vertexCount += 4;
                        indexCount += 2;
                    }
                }
            }
        }
    }

    UnloadMesh(chunkMesh);
    chunkMesh = {};

    chunkMesh.vertices = vertices;
    chunkMesh.texcoords = texcoords;
    chunkMesh.normals = normals;
    chunkMesh.indices = indices;

    chunkMesh.triangleCount = indexCount; // change it
    chunkMesh.vertexCount = vertexCount;

    UploadMesh(&chunkMesh, false);
    model = LoadModelFromMesh(chunkMesh);
    boundingBox = GetMeshBoundingBox(chunkMesh);
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


