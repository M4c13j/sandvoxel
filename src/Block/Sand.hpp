#pragma once
#include "block.hpp"

class Sand : public Block {
public:
    static Sand &getInstance() {
        static Sand singleton;
        return singleton;
    }

private:
    Sand() {
        type = BlockType::Sand;
        isTransparent = false;

        std::fill(std::begin(texcoords), std::end(texcoords), 0.0f);
        Color col = YELLOW;
        for (int i = 0; i < COLOR_DATA_PER_FACE * 6; i += 4) {
            colors[i]     = col.r;
            colors[i + 1] = col.g;
            colors[i + 2] = col.b;
            colors[i + 3] = col.a;
        }
    }
};


