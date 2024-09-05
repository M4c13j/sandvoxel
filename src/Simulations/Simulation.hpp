#pragma once

#include "block.hpp"

class World;

// Interface for Simulation implementations.
class Simulation {
protected:
    float  tickPerSecond  = 1.0f;
    int    tickTimeDiff   = 1.0f / tickPerSecond;
    double lastUpdateTime = 0.0f;

    World &world;

    Simulation(World &world) : world(world){};

public:
    virtual ~    Simulation()                  = default;
    virtual void addBlock(int x, int y, int z) = 0;
    virtual void addBlock(Cord cord)           = 0;
    virtual void update()                      = 0;
    bool         shouldUpdate() const { return GetTime() - lastUpdateTime > tickTimeDiff; }
};