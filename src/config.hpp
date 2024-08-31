#pragma once
#include "raylib.h"

namespace config {
constexpr int BLOCK_SIZE   = 16;
constexpr int BLOCK_HEIGHT = 16;

constexpr int CHUNK_SIZE           = 16; // chunks are cubes
constexpr int BLOCKS_IN_CHUNK      = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;
constexpr int MAP_SIDE_IN_CHUNKS   = 4; // assuming square map
constexpr int MAP_HEIGHT_IN_CHUNKS = 2;
constexpr int MAP_SIDE_IN_BLOCKS   = MAP_SIDE_IN_CHUNKS * CHUNK_SIZE;
constexpr int MAP_HEIGHT_IN_BLOCKS = MAP_HEIGHT_IN_CHUNKS * CHUNK_SIZE;
} // namespace config