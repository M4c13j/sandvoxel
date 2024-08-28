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
    // IMPORTANT: if World is stored in stack and it has more than ~60 chunks, it may fill whole stack (on my WSL 2
    // debian it is 8kb and it is standard). You have to allocate World on heap otherwise code may have segfault at the
    // begging of main :(.
    Chunk chunks[config::MAP_SIDE_IN_CHUNKS][config::MAP_HEIGHT_IN_CHUNKS][config::MAP_SIDE_IN_CHUNKS];
    static constexpr size_t side   = config::MAP_SIDE_IN_CHUNKS; // for now, strict size
    static constexpr size_t height = config::MAP_HEIGHT_IN_CHUNKS;
    Vector3                 drawOffset; // offset so that mesh is drawn correctly

         World();
    void generate_perlin_chunks(uint_fast32_t seed);
    // returns block from given cordinates
    Block &get_block(Cord cord) {
        return get_chunk(cord).get_block(cord.x % config::BLOCK_SIZE, cord.y % config::BLOCK_SIZE,
                                         cord.z % config::BLOCK_SIZE);
    }
    // TODO: uses array positions, not "actual" cordinates
    // returns Cord with index of chunk that pos is in.
    [[nodiscard]] Cord chunk_cord_from_position(Vector3 pos) const {
        return {static_cast<int>((pos.x - drawOffset.x) / config::CHUNK_SIZE),
                static_cast<int>((pos.y - drawOffset.y) / config::CHUNK_SIZE),
                static_cast<int>((pos.z - drawOffset.z) / config::CHUNK_SIZE)};
    }
    // returns Cord with index of chunk that pos is in.
    [[nodiscard]] Cord chunk_cord_from_position(int x, int y, int z) const {
        return {(x - static_cast<int>(drawOffset.x)) / config::CHUNK_SIZE,
                (y - static_cast<int>(drawOffset.y)) / config::CHUNK_SIZE,
                (z - static_cast<int>(drawOffset.z)) / config::CHUNK_SIZE};
    }
    // using array indexes
    Chunk &get_chunk_raw_access(int arrayX, int arrayY, int arrayZ) { return chunks[arrayX][arrayY][arrayZ]; }
    Chunk &get_chunk_raw_access(Cord arrayCord) { return chunks[arrayCord.x][arrayCord.y][arrayCord.z]; }
    // using array indexes
    Chunk &get_chunk(int x, int y, int z) { return get_chunk_raw_access(chunk_cord_from_position(x, y, z)); }
    Chunk &get_chunk(Cord cord) { return get_chunk_raw_access(chunk_cord_from_position(cord)); }
    void   setBlock(int x, int y, int z, Block::Type newType) { get_chunk(x,y,z).setBlockType(x%config::CHUNK_SIZE, y%config::CHUNK_SIZE, z%config::CHUNK_SIZE, newType); }
    void   mesh_all_chunks();
    void   mesh_chunk(Cord pos);
    void   draw_all(Texture &atlas, bool drawBoundingBox);
    void   print_size_report();
};

#define GENERATE_2D_VECTOR(_TYPE, _ROWS, _COLUMNS, _DEFAULT)                                                           \
    std::vector<std::vector<_TYPE>>(_ROWS, std::vector<_TYPE>(_COLUMNS, _DEFAULT))

#define GENERATE_3D_VECTOR(_TYPE, _ROWS, _COLUMNS, _HEIGHT, _DEFAULT)                                                  \
    std::vector<_TYPE>(_ROWS, std::vector<_TYPE>(_COLUMNS, std::vector<_TYPE>(_HEIGHT, _DEFAULT)))