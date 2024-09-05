#pragma once

#include "Simulation.hpp"

#include <deque>

class World;
class SandSimulation : Simulation {
    float  tickPerSecond  = 1; // simulation rounds per sceond
    int    tickTimeDiff   = 1.0f / tickPerSecond;
    double lastUpdateTime = 0.0f;

    std::deque<Cord> activeSand;

public:
    explicit SandSimulation(World &world) : Simulation(world){};
    void     addBlock(int x, int y, int z) override;
    void     addBlock(Cord cord) override;
    void     update() override;
    void     addSource(int x, int y, int z, int strenghInTicks);
    void removeInactive();

private:
    void clearCurrentBLock(Cord cord) const;
};