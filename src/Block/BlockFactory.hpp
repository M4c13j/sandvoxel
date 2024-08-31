#pragma once

#include "Air.hpp"
#include "Dirt.hpp"
#include "Sand.hpp"
#include "Fluid.hpp"

#include <cassert>

class BlockFactory {
public:
    static BlockFactory& getInstance() {
        static BlockFactory fact;
        return fact;
    }

    Block *getObjectFromType(BlockType type) const {
        assert(classesInitialised); // "Classes have not been initialised!"
        if (type == BlockType::Air) return new Air();
        if (type == BlockType::Sand) return new Sand();
        if (type == BlockType::Fluid) return new Fluid();
        assert(false);
    }

    void initBlocks() {
        Air().init();
        Sand().init();
        Fluid().init();
        classesInitialised = true;

        // if sizes dont match, create variable of maximal size
        assert(sizeof(Air) == sizeof(Sand));
    }

private:
    bool classesInitialised = false;

};
