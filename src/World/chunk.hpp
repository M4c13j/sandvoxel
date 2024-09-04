#pragma once

#include "../Block/BlockFactory.hpp"
#include "../config.hpp"
#include "raylib.h"

#include <vector>

// Whether mesh should be drawn with texture or nah
enum MeshType { MESH_TEXTURED = 0, MESH_COLORED };


enum DrawChunkFlags {
    DRAW_DONT_CHUNK_MODEL         = 1,      // tf that means, skip drawing mesh bro
    DRAW_BLOCK_WIRES              = 1 << 1, // every block wire
    DRAW_CHUNK_BOUNDING_BOX       = 1 << 2, // bound whole chunk area
    DRAW_CHUNK_DRAWN_BOUNDING_BOX = 1 << 3, // only bound what has been drawn
    DRAW_CHUNK_DEBUG_WIRES
    = DRAW_DONT_CHUNK_MODEL | DRAW_BLOCK_WIRES | DRAW_CHUNK_BOUNDING_BOX | DRAW_CHUNK_DRAWN_BOUNDING_BOX,
    DRAW_CHUNK_DEBUG_WIRES_MODEL
    = DRAW_BLOCK_WIRES | DRAW_CHUNK_BOUNDING_BOX | DRAW_CHUNK_DRAWN_BOUNDING_BOX
};


// Whether nonEmptyBlocks and visibleFaces make any sense BLOCKS in chunk TIMES faces
static_assert(config::BLOCKS_IN_CHUNK * 6 <= UINT16_MAX, "Total number of visible faces may not fit in uint16.");
class Chunk {
public:
    uint16_t    nonEmptyBlocks        = 0;
    uint16_t    visibleFaces          = 0;
    int         id                    = 0;
    MeshType    meshType              = MESH_COLORED;
    Cord        cords                 = {0, 0, 0}; // x,z - grid. y - height offset
    Vector3     drawPos               = {0, 0, 0}; // posotion of block in direction of {-1, -1, -1}.
    BoundingBox boundingBox           = {};
    Chunk      *neighbours[DIR_COUNT] = {nullptr};
    Model       model                 = {};

private:
    Block *blocks[config::CHUNK_SIZE][config::CHUNK_SIZE][config::CHUNK_SIZE]; // array of blocks of chunk (xyz)
    void               init_blocks();

public:
    Chunk() : Chunk((Cord){0, 0, 0}, 0) {}
    Chunk(Cord cords, int id) : id(id), cords(cords) {
        model        = LoadModelFromMesh({});
        model.meshes = new Mesh();
        init_blocks();
    }
    ~Chunk();

    void generate_default_blocks(int airLevel);
    void generate_perlin(uint_fast32_t seed);

    Block *get_block(int x, int y, int z) { return blocks[x][y][z]; }

    // Does nothing if newType is the same as current type of that block. Does not validate cordinates
    void setBlockType(int x, int y, int z, BlockType newType) {
        Block *curr = blocks[x][y][z];
        if (curr->getType() == newType)
            return;

        nonEmptyBlocks -= (newType == BlockType::Air && curr->getType() != BlockType::Air); // Solid to air then -1
        nonEmptyBlocks
            += (curr->getType() == BlockType::Air && newType != BlockType::Air); // Air to solid block then +1

        delete curr;
        blocks[x][y][z] = BlockFactory::getInstance().getObjectFromType(newType);
        assert(blocks[x][y][z]->getType() == newType);
    }

    bool isEmpty() const { return nonEmptyBlocks == 0; }
    bool isVisible() const { return !isEmpty() && visibleFaces != 0; }
    bool is_visible_face(Cord pos, Dir dir);

    void check_visible_faces(); // fills visible array of blocks
    void generate_mesh();
    // Chunk drawing flag are in DrawChunkFlags enum!. 0 draws just chunk, no debug
    void draw_chunk(Texture &text, DrawChunkFlags flags);
    static bool is_in_chunk(const Cord pos) {
        static_assert((config::CHUNK_SIZE & (config::CHUNK_SIZE - 1)) == 0); // chunk size is power of 2
        const int mask = ~(config::CHUNK_SIZE-1);
        return ((pos.x & mask) == 0) && ((pos.y & mask) == 0) && ((pos.z & mask) == 0);
    }
};


