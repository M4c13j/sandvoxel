#pragma once
#include "raylib.h"

namespace config {
    constexpr int BLOCK_SIZE = 16;
    constexpr int BLOCK_HEIGHT = 16;

    constexpr int MAP_SIZE = 256; // assuming square map
    constexpr int CHUNK_SIZE = 16; // assuming square chunk (x and z)
    constexpr int CHUNK_HEIGHT = 128; // height of chunk (y)


    constexpr Vector3 NORMALS[] = {
        {1, 0, 0},
        {-1, 0, 0},
        {0, 1, 0},
        {0, -1, 0},
        {0, 0, 1},
        {0, 0, -1}
    };

    Mesh CUBE_MESH = GenMeshCube(1.0f, 1.0f, 1.0f);
}