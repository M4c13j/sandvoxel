#include "Fluid.hpp"

bool      Fluid::fluid                                = true;
bool      Fluid::solid                                = false;
bool      Fluid::trans                                = false;
BlockType Fluid::type                                 = BlockType::Fluid;
u_char    Fluid::colors[COLOR_DATA_PER_FACE * 6]      = {0}; // TODO: wypełnić
float     Fluid::texcoords[TEXTURE_DATA_PER_FACE * 6] = {0};

void Fluid::init() {
    Color col = BLUE;
    for (int i = 0; i < COLOR_DATA_PER_FACE * 6; i += 4) {
        colors[i]     = col.r;
        colors[i + 1] = col.g;
        colors[i + 2] = col.b;
        colors[i + 3] = 100;
    }
}