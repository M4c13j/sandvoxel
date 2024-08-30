#pragma once

#include "Air.hpp"
#include "Dirt.hpp"
#include "Sand.hpp"

#include <cassert>

class BlockFactory {
public:
    static Block *getInstance(BlockType type) {
        if (type == BlockType::Air) return &Air::getInstance();
        if (type == BlockType::Sand) return &Sand::getInstance();
        assert(false);
        // if (type == Air) return Air::getInstance();
        return nullptr; // haha evil as hell
    }
};
