#pragma once

#include "../Block/Fluid.hpp"
#include "Simulation.hpp"
#include <vector>

#include <deque>

class World;

// Only active cells distribute water
// TODO: add evaporation
class FluidSimulation : Simulation {
    const float MAX_MASS     = 1.0f;
    const float MAX_COMPRESS = 0.02;

    const float MIN_MASS = 0.0001;
    const float MIN_FLOW = 0.01;

    const float MIN_DRAW = 0.01;
    const float MAX_DRAW = 1.1;

    const float MAX_SPEED = 1;

    std::deque<Cord> activeFluids;

public:
    explicit FluidSimulation(World &world) : Simulation(world, 1){};
    void     addBlockInitMass(int x, int y, int z, float mass);
    void     addBlockInitMass(Cord cord, float mass);
    void     addBlock(int x, int y, int z) override;
    void     addBlock(Cord cord) override;
    void     update() override;
    void     addSource(int x, int y, int z, int strenghInTicks);
    void     remove_empty_fluids();

private:
    void  update_blocks();
    float get_stable_state_b(float totalMass);
    void  add_additional_fluids();
};