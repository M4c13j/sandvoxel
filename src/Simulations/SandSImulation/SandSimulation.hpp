#pragma once

#include "Simulation.hpp"

#include <deque>

class World;
class SandSimulation : Simulation {
    std::deque<Cord> activeSand;

public:
    explicit SandSimulation(World &world) : Simulation(world, 2){};
    void     addBlock(int x, int y, int z) override;
    void     addBlock(Cord cord) override;
    void     update() override;
    void     addSource(int x, int y, int z, int strenghInTicks);
    void removeInactive();

private:
    void clearCurrentBLock(Cord cord) const;
};