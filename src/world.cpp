#include "world.hpp"
#include "raymath.h"
World::World() {
    float heightOffset = (-1) * config::MAP_HEIGHT / 2;
    float normalOffset = (-1) * (float)config::CHUNK_SIZE * side / 2;
    drawOffset = {normalOffset, heightOffset, normalOffset};

    for (int x = 0; x < config::MAP_SIDE; x++) {
        for (int y = 0; y < config::MAP_HEIGHT; y++) {
            for (int z = 0; z < config::MAP_SIDE; z++) {
                Chunk *curr = &chunks[x][y][z];
                curr->cords = {x,y,z};
                curr->id = -1; // TODO: remove or finish
                curr->drawPos = Vector3Add(
                    (Vector3){(float)x * config::CHUNK_SIZE, , (float)col * config::CHUNK_SIZE},
                    drawOffset);
                curr->neighbours[DIR_NORTH] = col+1 < side ? &chunks[row][col+1] : nullptr;
                curr->neighbours[DIR_SOUTH] = col-1 >= 0 ? &chunks[row][col-1] : nullptr;
                curr->neighbours[DIR_EAST] = row+1 < side ? &chunks[row+1][col] : nullptr;
                curr->neighbours[DIR_WEST] = row-1 >= 0 ? &chunks[row-1][col] : nullptr;
            }
        }
    }
}

void World::generate_default_chunks() {
    for (auto &row : chunks) {
        for (auto &chunk : row) {
            chunk.generate_default_blocks(-config::CHUNK_HEIGHT/2);
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

Chunk *World::get_chunk(int x, int y, int z) {
    assert((x < side && x >= 0) && (y < side && y >= 0) && (z < side && z >= 0));
    return &chunks[x][y][z];
}

void World::mesh_chunk(Cord pos) {
}
