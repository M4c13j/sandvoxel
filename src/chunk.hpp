#pragma once
#include "raylib.h"
#include "config.hpp"

class Chunk {
public:
    Vector3 cords = {0,0,0};
    int block[config::CHUNK_SIZE][config::CHUNK_HEIGHT][config::CHUNK_SIZE]; // array of blocks of chunk (xyz)
    int id = 0;
    Mesh chunkMesh;


    Chunk();
    void generate_default_blocks(int airLevel);
    void draw_chunk(Texture &text);
    void generate_mesh();
};
