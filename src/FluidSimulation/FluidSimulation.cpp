#include "FluidSimulation.hpp"
#include "../world.hpp"


void FluidSimulation::addFluid(int x, int y, int z) {
    activeFluids.push_back({x,y,z});
    world.setBlock(x, y, z, BlockType::Fluid);
}

void FluidSimulation::update(std::deque<Cord> &chunksToUpdate) {
    if (GetTime() - lastTick < tickTimeDiff)
        return; // do not update yet

    for (auto& fluid : activeFluids) {
        continue;
    }
}