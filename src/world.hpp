#pragma once
#include "FluidSimulation/FluidSimulation.hpp"
#include "chunk.hpp"
#include "config.hpp"
#include "raylib.h"
#include "raymath.h"

#include <array>
#include <deque>
#include <queue>

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
    static constexpr size_t side   = config::MAP_SIDE_IN_CHUNKS; // for now, strict size
    static constexpr size_t height = config::MAP_HEIGHT_IN_CHUNKS;
    Vector3                 drawOffset; // offset so that mesh is drawn correctly
    FluidSimulation         fluidSim;

private:
    Chunk            chunks[config::MAP_SIDE_IN_CHUNKS][config::MAP_HEIGHT_IN_CHUNKS][config::MAP_SIDE_IN_CHUNKS];
    std::deque<Cord> chunksToMesh; // CONTAINS CHUNK RAW CORDINATES

public:
         World();
    ~    World() = default;

    void generate_perlin_chunks(uint_fast32_t seed);

    // returns block from given cordinates(world cordinates)
    Block *get_block(int x, int y, int z) {
        return get_chunk(x, y, z).get_block(((x % config::CHUNK_SIZE) + 16) % config::CHUNK_SIZE,
                                            ((y % config::CHUNK_SIZE) + 16) % config::CHUNK_SIZE,
                                            ((z % config::CHUNK_SIZE) + 16) % config::CHUNK_SIZE);
    }
    Block *get_block_neigh(int x, int y, int z, Dir dir) {
        return get_block(x + FACE_NORMALS[dir].x, y + FACE_NORMALS[dir].y, z + FACE_NORMALS[dir].z);
    }
    std::array<Block*, DIR_COUNT> get_all_neighbours(int x, int y, int z);

    // TODO: uses array positions, not "actual" cordinates
    // returns Cord with index of chunk that pos is in.
    Cord chunk_cord_from_position(Vector3 pos) const {
        return {static_cast<int>((pos.x - drawOffset.x) / config::CHUNK_SIZE),
                static_cast<int>((pos.y - drawOffset.y) / config::CHUNK_SIZE),
                static_cast<int>((pos.z - drawOffset.z) / config::CHUNK_SIZE)};
    }

    // returns Cord with index of chunk that pos is in.
    Cord chunk_cord_from_position(int x, int y, int z) const {
        return {(x - static_cast<int>(drawOffset.x)) / config::CHUNK_SIZE,
                (y - static_cast<int>(drawOffset.y)) / config::CHUNK_SIZE,
                (z - static_cast<int>(drawOffset.z)) / config::CHUNK_SIZE};
    }

    // using array indexes
    Chunk &get_chunk_raw_access(int arrayX, int arrayY, int arrayZ) { return chunks[arrayX][arrayY][arrayZ]; }
    Chunk &get_chunk_raw_access(Cord arrayCord) { return chunks[arrayCord.x][arrayCord.y][arrayCord.z]; }

    // using array indexes
    Chunk &get_chunk(int x, int y, int z) { return get_chunk_raw_access(chunk_cord_from_position(x, y, z)); }
    Chunk &get_chunk(Cord cord) { return get_chunk_raw_access(chunk_cord_from_position(Vector3(cord))); }

    void   addFluid(int x, int y, int z);
    void   setBlock(int x, int y, int z, const BlockType newType) {
        get_chunk(x, y, z).setBlockType(((x % config::CHUNK_SIZE) + 16) % config::CHUNK_SIZE,
                                          ((y % config::CHUNK_SIZE) + 16) % config::CHUNK_SIZE,
                                          ((z % config::CHUNK_SIZE) + 16) % config::CHUNK_SIZE, newType);
    }

    void mesh_all_chunks();
    void update(); // update everything every frame
    void draw_all(Texture &atlas, DrawChunkFlags flags);

    void print_size_report() const;
    bool isInWorld(int x, int y, int z) const {
        return (drawOffset.x <= x) && (x < (-1) * drawOffset.x)
            && (drawOffset.y <= y) && (y < (-1) * drawOffset.y)
                && (drawOffset.z <= z) && (z < (-1) * drawOffset.z);
    }
};

#define GENERATE_2D_VECTOR(_TYPE, _ROWS, _COLUMNS, _DEFAULT)                                                           \
    std::vector<std::vector<_TYPE>>(_ROWS, std::vector<_TYPE>(_COLUMNS, _DEFAULT))

#define GENERATE_3D_VECTOR(_TYPE, _ROWS, _COLUMNS, _HEIGHT, _DEFAULT)                                                  \
    std::vector<_TYPE>(_ROWS, std::vector<_TYPE>(_COLUMNS, std::vector<_TYPE>(_HEIGHT, _DEFAULT)))