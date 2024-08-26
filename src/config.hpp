#pragma once
#include "raylib.h"

namespace config {
constexpr int BLOCK_SIZE   = 16;
constexpr int BLOCK_HEIGHT = 16;

constexpr int MAP_SIDE        = 256; // assuming square map
constexpr int MAP_HEIGHT      = 129;
constexpr int CHUNK_SIZE      = 16;         // assuming square chunk (x and z)
constexpr int CHUNK_HEIGHT    = CHUNK_SIZE; // height of chunk (y)
constexpr int BLOCKS_IN_CHUNK = CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE;
} // namespace config