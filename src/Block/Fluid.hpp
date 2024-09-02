#pragma once
#include "block.hpp"

#include <raymath.h>

class Fluid : public Block {
public:
    float mass    = 0;
    float newMass = 0;

              Fluid() = default;
    void      init() override;
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