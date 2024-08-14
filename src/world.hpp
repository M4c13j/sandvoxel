#pragma once
#include "raylib.h"
#include "raymath.h"
#include "config.hpp"
#include "chunk.hpp"

class World {
public:
    std::vector<std::vector<Chunk>> chunks;
    size_t x_size, z_size;
    Cord offset;

    World(int x, int y);
    void generate_default_chunks();
    void generate_perlin_chunks(uint_fast32_t seed);
    void mesh_all_chunks();
    void mesh_chunk(Cord pos);
    void draw_all(Texture &atlas);
};

#define GENERATE_2D_VECTOR(_TYPE, _ROWS, _COLUMNS, _DEFAULT) \
    std::vector<std::vector<_TYPE>> ( \
        _ROWS, \
        std::vector<_TYPE>(_COLUMNS, _DEFAULT) \
    )