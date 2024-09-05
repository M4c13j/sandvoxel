#pragma once

#include "block.hpp"

class World;

// Interface for Simulation implementations.
class Simulation {
protected:
    float  ticksPerSecond;
    int    tickTimeDiff;
    double lastUpdateTime;

    World &world;

    Simulation(World &world, float ticksPerSecond) : world(world), ticksPerSecond(ticksPerSecond) {
        tickTimeDiff   = 1.0 / ticksPerSecond;
        lastUpdateTime = 0.0f;
    };

public:
    virtual ~    Simulation()                  = default;
    virtual void addBlock(int x, int y, int z) = 0;
    virtual void addBlock(Cord cord)           = 0;
    virtual void update()                      = 0;
    bool         shouldUpdate() const { return GetTime() - lastUpdateTime > tickTimeDiff; }
};