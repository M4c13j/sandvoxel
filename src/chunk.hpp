#pragma once
#include "block.hpp"
#include "config.hpp"
#include "raylib.h"
// #include "world.hpp"
#include <vector>

class World;

class Chunk {
public:
    Cord    cords   = {0, 0, 0}; // x,z - grid. y - height offset
    Vector3 drawPos = {0, 0, 0}; // posotion of block in direction of {-1, -1, -1}.
    Block   blocks[config::CHUNK_SIZE][config::CHUNK_SIZE][config::CHUNK_SIZE]; // array of blocks of chunk (xyz)
    Chunk  *neighbours[DIR_COUNT] = {nullptr};
    int     id                    = 0;
    int     nonEmptyBlocks        = 0;
    Mesh    chunkMesh             = {};
    Model   model                 = {};

                  Chunk() = default;
    explicit      Chunk(Cord cords) : cords(cords) {}
                  Chunk(Cord cords, int id) : cords(cords), id(id) {}
    ~             Chunk();
    inline Block *get_block(int x, int y, int z) { return &blocks[x][y][z]; }
    void setBlockType(int x, int y, int z, Block::Type newType) {
        Block &curr = blocks[x][y][z];
        nonEmptyBlocks -= (newType == Block::Air && curr.type != Block::Air); // Solid to air then -1
        nonEmptyBlocks += (curr.type == Block::Air && newType != Block::Air); // Air to solid block then +1
        curr.setType(newType);
    }
    inline bool   isEmpty() const { return nonEmptyBlocks == 0; }
    bool          is_visible_face(Cord pos, Dir dir);

    int           set_visible_faces(); // fills visible array of blocks, returns num of visible faces
    void          update_visibility(); // whole chunk
    void          update_visibility_block(int x, int y, int z);
    void          generate_default_blocks(int airLevel);
    void          generate_perlin(uint_fast32_t seed);
    void          draw_chunk(Texture &text);
    void          generate_mesh();
    void gen_mesh_block(float *vertPt, float *texPt, float *normalPt, unsigned short *indiPt, Block &block, Vector3 pos,
                        int index); // obsolete, dont use it now. Generate face through blocks.
};

inline bool is_in_chunk(Cord pos) {
    return (pos.x < config::CHUNK_SIZE && pos.x >= 1) && (pos.y < config::CHUNK_SIZE && pos.y >= 0)
           && (pos.z < config::CHUNK_SIZE && pos.z >= 0);
}
