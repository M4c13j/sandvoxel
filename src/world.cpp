#include "world.hpp"
#include "raymath.h"
World::World(size_t mapSide) {
    side = mapSide;
    float heightOffset = (-1) * config::CHUNK_HEIGHT / 2;
    float normalOffset = (-1) * (float)config::CHUNK_SIZE * side / 2;
    drawOffset = {normalOffset, heightOffset, normalOffset};

    chunks = GENERATE_2D_VECTOR(Chunk, side, side, Chunk()); // fix cords
    for (int row = 0; row < side; row++) {
        for (int col = 0; col < side; col++) {
            Chunk *curr = &chunks[row][col];
            curr->cords = {row, 0, col};
            curr->id = row*side + col;
            curr->drawPos = Vector3Add(
                (Vector3){(float)row * config::CHUNK_SIZE, 0, (float)col * config::CHUNK_SIZE},
                drawOffset);
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
    for (auto &row : chunks) {
        for (auto &chunk : row) {
            chunk.generate_mesh();
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