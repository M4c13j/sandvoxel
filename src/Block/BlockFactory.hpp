#pragma once

#include "Air.hpp"
#include "Dirt.hpp"
#include "Sand.hpp"

#include <cassert>

class BlockFactory {
public:
    static BlockFactory& getInstance() {
        static BlockFactory fact;
        return fact;
    }

    Block *getObjectFromType(BlockType type) {
        assert(classesInitialised); // "Classes have not been initialised!"
        if (type == BlockType::Air) return new Air();
        if (type == BlockType::Sand) return new Sand();
        assert(false);
    }

    void initBlocks() {
        Air().init();
        Sand().init();
        classesInitialised = true;
    }

private:
    bool classesInitialised = false;

};
