#pragma once

#include "../Block/Fluid.hpp"
#include <vector>

#include <deque>

class World;
class FluidSimulation {
    const float MAX_MASS     = 1.0f;
    const float MAX_COMPRESS = 0.02;

    const float MIN_MASS = 0.0001;
    const float MIN_FLOW = 0.001;

    const float MIN_DRAW = 0.01;
    const float MAX_DRAW = 1.1;

    const float MAX_SPEED = 1;

    float  tickPerSecond  = 1.5; // simulation rounds per sceond
    int    tickTimeDiff   = 1.0f / tickPerSecond;
    double lastUpdateTime = 0.0f;

    World           &world;
    std::deque<Cord> activeFluids;

public:
    explicit FluidSimulation(World &world) : world(world){};
    void     addFluidInitMass(int x, int y, int z, float mass);
    void     addFluidInitMass(Cord cord, float mass);
    void     addFluid(int x, int y, int z);
    void     addFluid(Cord cord);
    void     update();
    bool     shouldUpdate() const { return GetTime() - lastUpdateTime > tickTimeDiff; }
    void     addSource(int x, int y, int z, int strenghInTicks);

private:
    void  update_blocks();
    float get_stable_state_b(float totalMass);
    void  add_additional_fluids();
};