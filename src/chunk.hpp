#pragma once
#include "raylib.h"
#include "config.hpp"
#include "block.hpp"

class Chunk {
public:
    Vector3 cords = {0,0,0};
    Block block[config::CHUNK_SIZE][config::CHUNK_HEIGHT][config::CHUNK_SIZE]; // array of blocks of chunk (xyz)
    int id = 0;
    Mesh chunkMesh;


    Chunk() {};
    Chunk(Vector3 cords) : cords(cords) {};
    Chunk(Vector3 cords, int id) : cords(cords), id(id) {};
    void generate_default_blocks(int airLevel);
    void check_visibility();
    void draw_chunk(Texture &text);
    void generate_mesh();
private:
    void gen_mesh_block(float *vertPt, float *texPt, float *normalPt,
        unsigned short *indiPt, Block &block, Vector3 pos, int index);
};
