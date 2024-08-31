#pragma once

#include "../Block/Fluid.hpp"
#include <vector>

#include <deque>

class World;
class FluidSimulation {
    int tickPerSecond = 10; // simulation rounds per sceond
    int tickTimeDiff = 1.0f / tickPerSecond;
    double lastTick = 0.0f;
    World &world;
    std::vector<Cord> activeFluids;

public:
    explicit FluidSimulation(World &world) : world(world) {};
    void addFluid(int x, int y, int z);
    void update(std::deque<Cord> &chunksToUpdate);
private:
    void update_blocks();
};