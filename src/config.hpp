#pragma once
#include "raylib.h"

namespace config {
    constexpr int BLOCK_SIZE = 16;
    constexpr int BLOCK_HEIGHT = 16;

    constexpr int MAP_SIZE = 256; // assuming square map
    constexpr int CHUNK_SIZE = 16; // assuming square chunk (x and z)
    constexpr int CHUNK_HEIGHT = 16; // height of chunk (y)
    constexpr int BLOCKS_IN_CHUNK = CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE;

    enum Directions {
        DIR_UP,
        DIR_DOWN,
        DIR_NORTH,
        DIR_SOUTH,
        DIR_EAST,
        DIR_WEST
    };

    constexpr Vector3 NORMALS[] = {
        {1, 0, 0},
        {-1, 0, 0},
        {0, 1, 0},
        {0, -1, 0},
        {0, 0, 1},
        {0, 0, -1}
    };

}