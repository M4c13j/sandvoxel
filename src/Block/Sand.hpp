#pragma once
#include "block.hpp"

class Sand : public Block {
public:
         Sand()          = default;
    void init() override {
        Color col = YELLOW;
        for (int i = 0; i < COLOR_DATA_PER_FACE * 6; i += 4) {
            colors[i]     = col.r;
            colors[i + 1] = col.g;
            colors[i + 2] = col.b;
            colors[i + 3] = col.a;
        }
    }
    bool      isTransparent() override { return trans; }
    BlockType getType() override { return type; }
    u_char   *getColors() override { return colors; }
    float    *getTexcoords() override { return texcoords; };

private:
    static bool      trans;
    static BlockType type;
    static u_char    colors[COLOR_DATA_PER_FACE * 6];
    static float texcoords[TEXTURE_DATA_PER_FACE * 6]; // texture data for every face (indexed by normal from given Dir
};