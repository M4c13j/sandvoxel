#pragma once

#include "Air.hpp"
#include "Dirt.hpp"
#include "Fluid.hpp"
#include "Grass.hpp"
#include "Sand.hpp"

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
        if (type == BlockType::Grass) return new Grass();
        if (type == BlockType::Sand) return new Sand();
        if (type == BlockType::Fluid) return new Fluid();
        assert(false);
    }

    void initBlocks() {
        // Initialize static data for each block type without creating temporary objects
        static Air air_instance;
        static Sand sand_instance;
        static Grass grass_instance;
        static Fluid fluid_instance;
        
        air_instance.init();
        sand_instance.init();
        grass_instance.init();
        fluid_instance.init();
        
        classesInitialised = true;

        // if sizes dont match, create variable of maximal size
        assert(sizeof(Air) == sizeof(Sand));
    }

private:
    bool classesInitialised = false;

};
