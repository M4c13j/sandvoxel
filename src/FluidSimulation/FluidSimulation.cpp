#include "FluidSimulation.hpp"
#include "../world.hpp"

void FluidSimulation::addFluidInitMass(int x, int y, int z, float mass) {
    activeFluids.push_back({x, y, z});
    world.setBlock(x, y, z, BlockType::Fluid);
    Fluid *newBLock   = dynamic_cast<Fluid *>(world.get_block(x, y, z));
    newBLock->mass    = mass;
    newBLock->newMass = mass;
}
void FluidSimulation::addFluidInitMass(Cord cord, float mass) { addFluidInitMass(cord.x, cord.y, cord.z, mass); }

void FluidSimulation::addFluid(int x, int y, int z) { addFluidInitMass(x, y, z, MAX_MASS); }

void FluidSimulation::addFluid(Cord cord) { addFluidInitMass(cord, MAX_MASS); }

static bool isAirOrFluid(BlockType type) { return type == BlockType::Fluid || type == BlockType::Air; }

// Remade function from processing library.
template<typename T>
static T constrain(T amt, T low, T high) {
    return std::min(high, std::max(low, amt));
}

void FluidSimulation::add_additional_fluids() {
    size_t initSize = activeFluids.size();
    const Dir DIRS_TO_EXPAND[] = {DIR_NORTH, DIR_SOUTH, DIR_DOWN, DIR_EAST, DIR_WEST};
    for (int i = 0; i < initSize; i++) {
        auto cord = activeFluids[i];

    }
}

// https://w-shadow.com/blog/2009/09/01/simple-fluid-simulation/
// https://tomforsyth1000.github.io/papers/cellular_automata_for_physical_modelling.html
void FluidSimulation::update(std::deque<Cord> &chunksToUpdate) {
    if (!shouldUpdate())
        return; // do not update yet
    lastUpdateTime = GetTime();

    puts("Fluid update...\n");


    size_t initFluids = activeFluids.size();
    for (int i = 0; i < initFluids; i++) {
        auto currCords = activeFluids.front(); activeFluids.pop_front(); activeFluids.push_back(currCords);
        auto* curr = dynamic_cast<Fluid *>(world.get_block(currCords.x, currCords.y, currCords.z));
        auto currType = curr->getType();

        if (currType != BlockType::Fluid) {
            continue; // FLuid has been repalced by block (or air)
        }

        // Actual simulation over fluids
        float flow = 0;
        float remainingMass = curr->mass;

        if (remainingMass <= 0) continue; // no water

        auto neighbours = world.get_all_neighbours(currCords.x, currCords.y, currCords.z);

        // Flow to block below
        Dir currentDir = DIR_DOWN;
        if (neighbours[currentDir] != nullptr && isAirOrFluid(neighbours[currentDir]->getType())) {
            Fluid *currNeigh = dynamic_cast<Fluid *>(neighbours[currentDir]);
            if (neighbours[currentDir]->getType() == BlockType::Air) {
                addFluidInitMass(currCords + FACE_NORMALS[currentDir], 0.0f);
                currNeigh
                    = dynamic_cast<Fluid *>(world.get_block_neigh(currCords.x, currCords.y, currCords.z, currentDir));
            }

            float currNeighInitMass = currNeigh->mass;

            flow = get_stable_state_b(remainingMass + currNeighInitMass) - currNeighInitMass;
            if (flow > MIN_FLOW) flow *= 0.5;

            flow = constrain(flow, 0.0f, std::min(MAX_SPEED, remainingMass));

            curr->newMass -= flow;
            currNeigh->newMass += flow;
            remainingMass -= flow;
        }

        if (remainingMass <= 0) continue;

        // Loop over all XZ dirs and calculate stuff
        constexpr Dir flatDirs[] = {DIR_NORTH, DIR_SOUTH, DIR_EAST, DIR_WEST};
        for (auto planarDir : flatDirs) {
            assert( remainingMass > 0);
            if (neighbours[planarDir] != nullptr && isAirOrFluid(neighbours[planarDir]->getType())) {
                Fluid *currNeigh = dynamic_cast<Fluid *>(neighbours[planarDir]);
                if (neighbours[planarDir]->getType() == BlockType::Air) {
                    addFluidInitMass(currCords + FACE_NORMALS[planarDir], 0.0f);
                    currNeigh
                        = dynamic_cast<Fluid *>(world.get_block_neigh(currCords.x, currCords.y, currCords.z, planarDir));
                }

                float currNeighInitMass = currNeigh->mass;

                flow = (curr->mass - currNeighInitMass) / 4.0f;
                if (flow > MIN_FLOW) flow *= 0.5;

                flow = constrain(flow, 0.0f, remainingMass);

                curr->newMass -= flow;
                currNeigh->newMass += flow;
                remainingMass -= flow;
            }

            if (remainingMass <= 0) break;
        }
    }

    int fluidsToCheck = activeFluids.size();
    for (int i = 0; i < fluidsToCheck; i++) {
        auto akt = activeFluids.front(); activeFluids.pop_front();
        Fluid* currFluid = dynamic_cast<Fluid*>(world.get_block(akt.x, akt.y, akt.z));
        currFluid->mass = currFluid->newMass;

        // Remove fluid if its mass is less than minimal
        if (currFluid->mass < MIN_MASS) {
            world.setBlock(akt.x, akt.y, akt.z, BlockType::Air);
        }
        else {
            activeFluids.push_back(akt);
        }
    }
        /*for (int dir = 0; dir < DIR_COUNT; dir++) {
            Cord neighPos = addNormalDir(currCords, static_cast<Dir>(dir));
            auto neighbour = world.get_block(neighPos.x, neighPos.y, neighPos.z);
            if (neighbour->getType() == BlockType::Air) {
                if (curr->mass > 1) {
                    addFluid(neighPos.x, neighPos.y, neighPos.z);
                }
            }
        }*/
}

float FluidSimulation::get_stable_state_b(float totalMass) {
    if (totalMass <= 1) {
        return 1;
    } else if (totalMass < 2 * MAX_MASS + MAX_COMPRESS) {
        return (MAX_MASS * MAX_MASS + totalMass * MAX_COMPRESS) / (MAX_MASS + MAX_COMPRESS);
    } else {
        return (totalMass + MAX_COMPRESS) / 2;
    }
}