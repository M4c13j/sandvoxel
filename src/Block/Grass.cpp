#include "Grass.hpp"

bool      Grass::solid                                = true;
bool      Grass::fluid                                = false;
bool      Grass::trans                                = false;
BlockType Grass::type                                 = BlockType::Grass;
u_char    Grass::colors[COLOR_DATA_PER_FACE * 6]      = {0}; // TODO: wypełnić
float     Grass::texcoords[TEXTURE_DATA_PER_FACE * 6] = {0};