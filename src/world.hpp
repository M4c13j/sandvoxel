#pragma once
#include "chunk.hpp"
#include "config.hpp"
#include "raylib.h"
#include "raymath.h"

/// World is made of chunks that are made of blocks
/// Chunks have GRID CORDINATES and blocks in world
/// have absolute and relative cordinates.
/// World is centered at (0,0,0) relative to raylib's coridantes.
/// Chunks are accessed the same way as the fields (x,y,z) are cordinates in
/// vector. Direct chunk access TODO is possible only via 3d vector.
///
class World {
public:
    Chunk chunks[config::MAP_SIDE_IN_CHUNKS][config::MAP_HEIGHT_IN_CHUNKS][config::MAP_SIDE_IN_CHUNKS];
    static constexpr size_t side   = config::MAP_SIDE_IN_CHUNKS; // for now, strict size
    static constexpr size_t height = config::MAP_HEIGHT_IN_CHUNKS;
    Vector3                 drawOffset; // offset so that mesh is drawn correctly

         World();
    void generate_default_chunks();
    void generate_perlin_chunks(uint_fast32_t seed);

    Block *get_block(Cord cord) { // returns block from given cordinates
        Cord relative{cord.x % config::CHUNK_SIZE, cord.y % config::CHUNK_SIZE, cord.z % config::CHUNK_SIZE};
        return get_chunk(cord.x / config::CHUNK_SIZE, cord.y / config::CHUNK_SIZE, cord.z / config::CHUNK_SIZE)
            ->get_block(relative.x, relative.y, relative.z);
    }
    // TODO: uses array positions, not "actual" cordinates
    Chunk *get_chunk(int x, int y, int z) { return &chunks[x][y][z]; }
    void   mesh_all_chunks();
    void   mesh_chunk(Cord pos);
    void   draw_all(Texture &atlas);
};

#define GENERATE_2D_VECTOR(_TYPE, _ROWS, _COLUMNS, _DEFAULT)                                                           \
    std::vector<std::vector<_TYPE>>(_ROWS, std::vector<_TYPE>(_COLUMNS, _DEFAULT))

#define GENERATE_3D_VECTOR(_TYPE, _ROWS, _COLUMNS, _HEIGHT, _DEFAULT)                                                  \
    std::vector<_TYPE>(_ROWS, std::vector<_TYPE>(_COLUMNS, std::vector<_TYPE>(_HEIGHT, _DEFAULT)))