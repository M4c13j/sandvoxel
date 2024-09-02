#include <cstdlib>
#include <cassert>
#include <algorithm>
#include <cstdio>

#include "../Block/BlockFactory.hpp"
#include "chunk.hpp"
#include "../config.hpp"
#include "../perlin.hpp"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include <complex>

const Cord FACE_NORMALS_CORD[] = {
    (Cord){0, 0, 1},
    (Cord){0, 0,-1},
    (Cord){0, 1, 0},
    (Cord){0,-1, 0},
    (Cord){1, 0, 0},
    (Cord){-1, 0, 0}
};

void Chunk::init_blocks() {
    for (int i = 0; i < config::CHUNK_SIZE; i++) {
        for (int j = 0; j < config::CHUNK_SIZE; j++) {
            for (int k = 0; k < config::CHUNK_SIZE; k++) {
                blocks[i][j][k] = new Air;
            }
        }
    }
}

Chunk::~Chunk() {
    // UnloadModel(model);
}

void Chunk::generate_default_blocks(int airLevel) {
    for (int x = 0; x < config::CHUNK_SIZE; x++) {
        for (int y = 0; y < config::CHUNK_SIZE; y++) {
            for (int z = 0; z < config::CHUNK_SIZE; z++) {
                if (y< airLevel)
                    // blocks[x][y][z] = new Sand();
                        setBlockType(x,y,z, BlockType::Sand);
                else
                    setBlockType(x,y,z, BlockType::Air);
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
                    setBlockType(x,y,z, BlockType::Sand);
                } else {
                    setBlockType(x,y,z, BlockType::Air);
                }
            }
        }
    }
}

/// If chunk model has a texture mapped to it, it would be drawn and color from mesh will tint its color.
/// To draw mesh only from colors defined in mesh.colors you have to set materials texture to null and set color to
/// WHITE (or other, it will act as TINT for colors defined in mesh).
void Chunk::draw_chunk(Texture &text, DrawChunkFlags flags) {
    if (!isVisible())
        return; // empty chunk, don't waste time

    // model.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = WHITE;
    // model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = text; // FOR TEXTURED MESH
    bool drawChunkModel = !(flags & DRAW_DONT_CHUNK_MODEL);
    if (drawChunkModel) DrawModel(model, {0,0,0}, 1, WHITE);
    if (flags & DRAW_BLOCK_WIRES) DrawModelWires(model, {0,0,0}, 1.0f, BLACK);
    if (flags & DRAW_CHUNK_DRAWN_BOUNDING_BOX) DrawBoundingBox(boundingBox, BLACK);
    if (flags & DRAW_CHUNK_BOUNDING_BOX) {
        Vector3 cubeDrawPos = Vector3Add(drawPos, Vector3Scale({1,1,1}, config::CHUNK_SIZE/2));
        DrawCubeWires(cubeDrawPos, config::CHUNK_SIZE, config::CHUNK_SIZE, config::CHUNK_SIZE, BLUE);
    }
    // UnloadMaterial(material);
    // UnloadModel(model);
}

// Returns visibilit of block at dir from pos. CHecks neighbouring chunk if needed.
bool Chunk::is_visible_face(const Cord pos, const Dir dir) {
    Cord nextPos
        = {pos.x + FACE_NORMALS_CORD[dir].x, pos.y + FACE_NORMALS_CORD[dir].y, pos.z + FACE_NORMALS_CORD[dir].z};
    if (is_in_chunk(nextPos)) {
        return get_block(nextPos.x, nextPos.y, nextPos.z)->isTransparent();
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

    return neighbours[dir]->get_block(nextPos.x, nextPos.y, nextPos.z)->isTransparent();
}

void Chunk::check_visible_faces() {
    int visCount = 0;
    for (int x = 0; x < config::CHUNK_SIZE; x++) {
        for (int y = 0; y < config::CHUNK_SIZE; y++) {
            for (int z = 0; z < config::CHUNK_SIZE; z++) {
                Block *curr  = blocks[x][y][z];
                curr->visible = 0;
                if (curr->isTransparent())
                    continue; // do not draw Air

                Cord pos{x, y, z};
                for (int dir = 0; dir < DIR_COUNT; dir++) {
                    if (is_visible_face(pos, static_cast<Dir>(dir))) {
                        curr->visible |= 1 << dir;
                        visCount++;
                    }
                }
            }
        }
    }
    visibleFaces = visCount;
}

void Chunk::generate_mesh() {
    if (isEmpty())
        return; // why bother?

    check_visible_faces(); // find all visible faces if there are any
    if (visibleFaces == 0)
        return; // do not allocate memory or other things if there ar no face visible.

    // constexpr int BLOCKS_IN_CHUNK = config::CHUNK_HEIGHT * config::CHUNK_SIZE * config::CHUNK_SIZE;
    const int VERTEX_DATA_TOTAL  = VERTEX_DATA_PER_FACE * visibleFaces;
    const int TEXTURE_DATA_TOTAL = TEXTURE_DATA_PER_FACE * visibleFaces;
    const int INDICES_DATA_TOTAL = INDICES_DATA_PER_FACE * visibleFaces;
    const int COLOR_DATA_TOTAL   = COLOR_DATA_PER_FACE * visibleFaces;


    auto *vertices  = static_cast<float *>(RL_MALLOC(VERTEX_DATA_TOTAL * sizeof(float)));
    auto *texcoords = static_cast<float *>(RL_MALLOC(TEXTURE_DATA_TOTAL * sizeof(float)));
    auto *normals   = static_cast<float *>(RL_MALLOC(VERTEX_DATA_TOTAL * sizeof(float)));
    auto *indices   = static_cast<unsigned short *>(RL_MALLOC(INDICES_DATA_TOTAL * sizeof(unsigned short)));
    auto *colors    = static_cast<unsigned char *>(RL_MALLOC(COLOR_DATA_TOTAL * sizeof(unsigned char)));

    FacePlacementData placementData = {0, vertices, texcoords, normals, indices, colors};
    int vertexCount = 0;
    int indexCount = 0;
    int usedFaces = 0;
    // wish I could foreach or have flexibility from java to use queue as array.
    for (int x = 0; x < config::CHUNK_SIZE; x++) {
        for (int y = 0; y < config::CHUNK_SIZE; y++) {
            for (int z = 0; z < config::CHUNK_SIZE; z++) {
                Block *curr = get_block(x, y, z);

                for (int dir = 0; dir < DIR_COUNT; dir++) {
                    if (curr->visible & (1 << dir)) {
                        Cord blockActualPos = Cord::fromVec3({drawPos.x + static_cast<float>(x), drawPos.y + static_cast<float>(y),
                                               drawPos.z + static_cast<float>(z)});
                        curr->generate_face(placementData, static_cast<Dir>(dir), blockActualPos);
                        placementData.advance_face();
                        vertexCount += 4;
                        indexCount += 2;
                        usedFaces++;
                    }
                }
            }
        }
    }

    UnloadMesh(model.meshes[0]);
    model.meshes[0] = Mesh{};
    Mesh &chunkMeshRef = model.meshes[0];

    chunkMeshRef.vertices = vertices;
    chunkMeshRef.texcoords = texcoords;
    chunkMeshRef.normals = normals;
    chunkMeshRef.indices = indices;
    chunkMeshRef.colors = colors;
    chunkMeshRef.triangleCount = indexCount; // change it
    chunkMeshRef.vertexCount = vertexCount;

    UploadMesh(model.meshes, false);
    // boundingBox = GetMeshBoundingBox(chunkMeshRef);
}
