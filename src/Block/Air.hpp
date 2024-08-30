#pragma once
#include "block.hpp"

class Air : public Block {
public:
    static Air &getInstance() {
        static Air singleton;
        return singleton;
    }

private:
    Air() {
        type = BlockType::Air;
        isTransparent = true;

        std::fill(std::begin(texcoords), std::end(texcoords), 0.0f);
        std::fill(std::begin(colors), std::end(colors), 0.0f);
    }
};