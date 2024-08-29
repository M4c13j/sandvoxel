#pragma once

#include "block.hpp"
#include "config.hpp"
#include "raylib.h"
// #include "world.hpp"
#include <vector>

// Whether nonEmptyBlocks and visibleFaces make any sense BLOCKS in chunk TIMES faces
static_assert(config::BLOCKS_IN_CHUNK * 6 <= UINT16_MAX, "Total number of visible faces may not fit in uint16.");
class Chunk {
public:
    uint16_t    nonEmptyBlocks        = 0;
    uint16_t    visibleFaces          = 0;
    int         id                    = 0;
    Cord        cords                 = {0, 0, 0}; // x,z - grid. y - height offset
    Vector3     drawPos               = {0, 0, 0}; // posotion of block in direction of {-1, -1, -1}.
    BoundingBox boundingBox           = {};
    Chunk      *neighbours[DIR_COUNT] = {nullptr};
    Model       model                 = {};
    Block       blocks[config::CHUNK_SIZE][config::CHUNK_SIZE][config::CHUNK_SIZE]; // array of blocks of chunk (xyz)

    Chunk() : blocks{} {
        model = LoadModelFromMesh({});
        model.meshes = new Mesh();
    }
    Chunk(Cord cords, int id) : id(id), cords(cords), blocks{} {
        model = LoadModelFromMesh({});
        model.meshes = new Mesh();
    }
    ~      Chunk();
    Block &get_block(int x, int y, int z) { return blocks[x][y][z]; }
    void   setBlockType(int x, int y, int z, Block::Type newType) {
        Block &curr = blocks[x][y][z];
        nonEmptyBlocks -= (newType == Block::Air && curr.type != Block::Air); // Solid to air then -1
        nonEmptyBlocks += (curr.type == Block::Air && newType != Block::Air); // Air to solid block then +1
        curr.setType(newType);
    }
    [[nodiscard]] bool isEmpty() const { return nonEmptyBlocks == 0; }
    [[nodiscard]] bool isVisible() const { return !isEmpty() && visibleFaces != 0; }
    bool               is_visible_face(Cord pos, Dir dir);

    void  check_visible_faces(); // fills visible array of blocks
    void update_visibility_block(int x, int y, int z);
    void generate_default_blocks(int airLevel);
    void generate_perlin(uint_fast32_t seed);
    void draw_chunk(Texture &text, bool drawBoundingBox);
    void generate_mesh();
    void gen_mesh_block(float *vertPt, float *texPt, float *normalPt, unsigned short *indiPt, Block &block, Vector3 pos,
                        int index); // obsolete, dont use it now. Generate face through blocks.
};

inline bool is_in_chunk(const Cord pos) {
    return (pos.x < config::CHUNK_SIZE && pos.x >= 0) && (pos.y < config::CHUNK_SIZE && pos.y >= 0)
           && (pos.z < config::CHUNK_SIZE && pos.z >= 0);
}
