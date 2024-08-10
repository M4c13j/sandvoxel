#pragma once
#include "raylib.h"

namespace config {
    constexpr int BLOCK_SIZE = 16;
    constexpr int BLOCK_HEIGHT = 16;

    constexpr int MAP_SIZE = 256; // assuming square map
    constexpr int CHUNK_SIZE = 16; // assuming square chunk (x and z)
    constexpr int CHUNK_HEIGHT = 16; // height of chunk (y)
    constexpr int BLOCKS_IN_CHUNK = CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE;

    // enum Dir {
    //     DIR_SOUTH = 0,
    //     DIR_NORTH,
    //     DIR_UP,
    //     DIR_DOWN,
    //     DIR_EAST,
    //     DIR_WEST,
    //     COUNT_DIR
    // };

    // const int VERTEX_DATA_PER_FACE = 3 * 4; // for vertices and normals
    // const int TEXTURE_DATA_PER_FACE = 2 * 4;
}