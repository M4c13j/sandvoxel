#include "world.hpp"

World::World(int x, int z) {
    x_size = x;
    z_size = z;
    offset = {0,0,0};

    chunks = GENERATE_2D_VECTOR(Chunk, x, z, Chunk()); // fix cords
    for (int row = 0; row < x; row++) {
        for (int col = 0; col < z; col++) {
            Chunk *curr = &chunks[row][col];
            curr->cords = {row * config::CHUNK_SIZE, 0, col * config::CHUNK_SIZE};
            curr->id = row*x + col;
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