#pragma once
#include "raylib.h"
#include "config.hpp"
#include "block.hpp"
// #include "world.hpp"
#include <vector>

class World;

class Chunk {
public:
    Cord cords = {0, 0, 0}; // x,z - grid. y - height offset
    Vector3 drawPos = {0, 0, 0}; // posotion of block in direction of {-1, -1, -1}.
    Block blocks[config::CHUNK_SIZE][config::CHUNK_HEIGHT][config::CHUNK_SIZE]; // array of blocks of chunk (xyz)
    Chunk* neighbours[DIR_COUNT] = {nullptr};
    int id = 0;
    Mesh chunkMesh = {0};
    Model model;

    Chunk() {};
    Chunk(Cord cords) : cords(cords) {};
    Chunk(Cord cords, int id) : cords(cords), id(id) {};
    ~Chunk() {}
    void generate_default_blocks(int airLevel);
    void generate_perlin(uint_fast32_t seed);
    inline Block *get_block(Cord pos) {return &blocks[pos.x][pos.y][pos.z];}
    bool is_visible_face(Cord pos, Dir dir);
    int set_visible_faces(); // fills visible array of blocks, returns num of visible faces
    void update_visibility(); // whole chunk
    void update_visibility_block(int x, int y, int z);
    void draw_chunk(Texture &text);
    void generate_mesh();
    void gen_mesh_block(float *vertPt, float *texPt, float *normalPt,
        unsigned short *indiPt, Block &block, Vector3 pos, int index);
};

inline bool is_in_chunk(Cord pos) {
    return (pos.x < config::CHUNK_SIZE && pos.x >= 0) &&
        (pos.y < config::CHUNK_HEIGHT && pos.y >= 0) &&
        (pos.z < config::CHUNK_SIZE && pos.z >= 0);
}
