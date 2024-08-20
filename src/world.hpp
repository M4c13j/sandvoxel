#pragma once
#include "raylib.h"
#include "raymath.h"
#include "config.hpp"
#include "chunk.hpp"

/// World is made of chunks that are made of blocks
/// Chunks have GRID CORDINATES and blocks in world
/// have absolute and relative cordinates.
/// World is CENTERED AT 0.
///
class World {
public:
    std::vector<std::vector<Chunk>> chunks;
    size_t side;
    Vector3 drawOffset; // offset so that mesh is drawn correctly

    World(size_t mapSide);
    void generate_default_chunks();
    void generate_perlin_chunks(uint_fast32_t seed);
    Block& get_block_cords(Cord cord); // returns block from given cordinates
    void mesh_all_chunks();
    void mesh_chunk(Cord pos);
    void draw_all(Texture &atlas);
};

#define GENERATE_2D_VECTOR(_TYPE, _ROWS, _COLUMNS, _DEFAULT) \
    std::vector<std::vector<_TYPE>> ( \
        _ROWS, \
        std::vector<_TYPE>(_COLUMNS, _DEFAULT) \
    )