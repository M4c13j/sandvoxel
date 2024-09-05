#include "SandSimulation.hpp"
#include "../World/world.hpp"
#include "block.hpp"

#include <chrono>
#include <random>

void SandSimulation::addBlock(int x, int y, int z) {
    world.setBlock(x, y, z, BlockType::Sand);
    activeSand.emplace_back(x,y,z);
}

void SandSimulation::addBlock(Cord cord) { addBlock(cord.x, cord.y, cord.z); }

// Random number from range [a,b] (inclusive)
static int randint(int a, int b) {
    // TODO: move  to utils file
    static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distribution(a, b);
    return distribution(rng);
}

void SandSimulation::clearCurrentBLock(Cord curr) const { world.setBlock(curr.x, curr.y, curr.z, BlockType::Air); }

void SandSimulation::update() {
    if (!this->shouldUpdate())
        return; // do not update yet
    lastUpdateTime = GetTime();

    size_t activeSandCount = activeSand.size();
    for (int i = 0; i < activeSandCount; i++) {
        // Cord  currCords     = activeSand[i];
        Cord currCords = activeSand.front(); activeSand.pop_front();
        auto *currBlockTemp = world.get_block(currCords.x, currCords.y, currCords.z);
        auto  currType      = currBlockTemp->getType();

        if (currType != BlockType::Sand) {
            continue; // Sand has been repalced by block (or air)
        }

        auto *curr = dynamic_cast<Sand*>(world.get_block(currCords.x, currCords.y, currCords.z));

        // Air below, free fall
        if (world.get_block_neigh(currCords.x, currCords.y, currCords.z, DIR_DOWN)->getType() == BlockType::Air) {
            clearCurrentBLock(currCords);
            currCords.add_dir(DIR_DOWN);
            addBlock(currCords);
            currCords.add_dir(DIR_UP);
            continue;
        }

        // Now: There is block directly below us, we need to choose direction to fall if there is any.
        std::vector<Cord> possibleDirs;
        possibleDirs.reserve(9);

        for (int dx = -1; dx <= 1; dx++) {
            for (int dz = -1; dz <= 1; dz++) {
                if (dx == 0 && dx == 0)
                    continue;
                Cord subDir = currCords + (Cord){dx, -1, dz};
                auto bl = world.get_block(subDir.x, subDir.y, subDir.z);
                auto ab = world.get_block(subDir.x, subDir.y +1, subDir.z);
                if (bl->getType() == BlockType::Air && ab->getType() == BlockType::Air) {
                    possibleDirs.push_back(subDir);
                }
            }
        }

        // all blocks below are solid
        if (possibleDirs.size() == 0) {
            continue; // stay where you are
        }

        Cord cordToFall = possibleDirs[randint(0, possibleDirs.size()-1)];
        assert(world.get_block(cordToFall.x, cordToFall.y, cordToFall.z)->getType() == BlockType::Air);
        clearCurrentBLock(currCords);
        addBlock(cordToFall);
    }

    removeInactive();
}

void SandSimulation::removeInactive() {
    size_t toCheck= activeSand.size();
    for (int i = 0; i < toCheck; i++) {
        auto akt = activeSand.front();
        activeSand.pop_front();
        Block* currBlock = world.get_block(akt.x, akt.y, akt.z);

        // Block is still a Sand, let's go
        if (currBlock->getType() == BlockType::Sand) {
            activeSand.push_back(akt);
        }
    }
}


