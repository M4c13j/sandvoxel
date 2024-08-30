#include "world.hpp"

#include "perlin.hpp"
#include "raymath.h"

#include <iostream>

void World::print_size_report() const {
    std::cout << "=========== SIZE report ============" << std::endl;
    std::cout << "Total chunks: " << side << "^2 * " << height << " = " << side*side*height <<"\n";
    std::cout << "World size: " << sizeof(*this) << " bytes  ~ " << sizeof(*this) / 1e6 << " mb\n";
    std::cout << "Chunk size: " << sizeof(chunks[0][0][0]) << " bytes \n";
    std::cout << "Block size: " << sizeof(chunks[0][0][0].blocks[0][0][0]) << " bytes \n";
}

World::World() {
    float heightOffset = (-1) * config::MAP_HEIGHT_IN_BLOCKS / 2;
    float normalOffset = (-1) * (float)config::CHUNK_SIZE * side / 2;
    drawOffset         = {normalOffset, heightOffset, normalOffset};

    for (int x = 0; x < config::MAP_SIDE_IN_CHUNKS; x++) {
        for (int y = 0; y < config::MAP_HEIGHT_IN_CHUNKS; y++) {
            for (int z = 0; z < config::MAP_SIDE_IN_CHUNKS; z++) {
                Chunk &curr                = chunks[x][y][z];
                curr.cords                 = {x, y, z};
                curr.id                    = -1; // TODO: remove or finish
                curr.drawPos               = Vector3Add((Vector3){static_cast<float>(x) * config::CHUNK_SIZE,
                                                                  static_cast<float>(y) * config::CHUNK_SIZE,
                                                                  static_cast<float>(z) * config::CHUNK_SIZE},
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
    const siv::PerlinNoise::seed_type seedG = seed; //(uint_fast16_t) GetMousePosition().x;
    const siv::PerlinNoise            perlin{seedG};
    const float                       PRECISION_FOR_PERLIN = 1.0 / (side * 8); // * config::CHUNK_SIZE);
    const int                         MAP_HEIGHT_BLOCKS    = height * config::CHUNK_SIZE;

    for (int x = 0; x < side; x++) {
        for (int z = 0; z < side; z++) {
            for (int bx = 0; bx < config::CHUNK_SIZE; bx++) {
                for (int bz = 0; bz < config::CHUNK_SIZE; bz++) {
                    int glevel = perlin.noise2D_01((bx + x * config::CHUNK_SIZE) * PRECISION_FOR_PERLIN,
                                                   (bz + z * config::CHUNK_SIZE) * PRECISION_FOR_PERLIN)
                                 * MAP_HEIGHT_BLOCKS;
                    for (int by = 0; by < MAP_HEIGHT_BLOCKS; by++) {
                        chunks[x][by / config::CHUNK_SIZE][z].setBlockType(bx, by % config::CHUNK_SIZE, bz,
                                                                           by > glevel ? BlockType::Air : BlockType::Sand);
                    }
                }
            }
        }
    }
}

void World::mesh_all_chunks() {
    for (auto &plane: chunks) {
        for (auto &row : plane) {
            for (auto &chunk : row) {
                chunk.generate_mesh();
            }
        }
    }
}

void World::draw_all(Texture &atlas, bool drawBoundingBox) {
    for (auto &plane: chunks) {
        for (auto &row : plane) {
            for (auto &chunk : row) {
                chunk.draw_chunk(atlas, drawBoundingBox);
            }
        }
    }

}


void World::mesh_chunk(Cord pos) {
}
