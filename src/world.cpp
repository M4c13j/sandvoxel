#include "world.hpp"
#include "raymath.h"

World::World() {
    float heightOffset = (-1) * config::MAP_HEIGHT_IN_BLOCKS / 2;
    float normalOffset = (-1) * (float)config::CHUNK_SIZE * side / 2;
    drawOffset         = {normalOffset, heightOffset, normalOffset};

    for (int x = 0; x < config::MAP_SIDE_IN_CHUNKS; x++) {
        for (int y = 0; y < config::MAP_HEIGHT_IN_CHUNKS; y++) {
            for (int z = 0; z < config::MAP_SIDE_IN_CHUNKS; z++) {
                Chunk &curr  = chunks[x][y][z];
                curr.cords   = {x, y, z};
                curr.id      = -1; // TODO: remove or finish
                curr.drawPos = Vector3Add((Vector3){(float)x * config::CHUNK_SIZE, (float)y * config::CHUNK_SIZE,
                                                    (float)z * config::CHUNK_SIZE},
                                          drawOffset);
                curr.neighbours[DIR_NORTH] = z + 1 < side ? &chunks[x][y][z + 1] : nullptr;
                curr.neighbours[DIR_SOUTH] = z - 1 >= 0 ? &chunks[x][y][z - 1] : nullptr;
                curr.neighbours[DIR_EAST]  = x + 1 < side ? &chunks[x + 1][y][z] : nullptr;
                curr.neighbours[DIR_WEST]  = x - 1 >= 0 ? &chunks[x - 1][y][z] : nullptr;
                curr.neighbours[DIR_UP]    = y + 1 < height ? &chunks[x][y + 1][z] : nullptr;
                curr.neighbours[DIR_DOWN]  = y - 1 >= 0 ? &chunks[x][y - 1][z] : nullptr;
            }
        }
    }
}


void World::generate_perlin_chunks(uint_fast32_t seed) {
    for (auto &row : chunks) {
        for (auto &chunk : row) {
            chunk.generate_perlin(seed);
        }
    }
}

void World::mesh_all_chunks() {
    for (auto &col : chunks) {
        for (auto &row : col) {
            for (auto &chunk : row) {
                chunk.generate_mesh();
            }
        }
    }
}

void World::draw_all(Texture &atlas) {
    for (auto &row : chunks) {
        for (auto &chunk : row) {
            chunk.draw_chunk(atlas);
        }
    }
}


void World::mesh_chunk(Cord pos) {
}
