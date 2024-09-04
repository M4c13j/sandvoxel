#include "Air.hpp"

bool      Air::solid                                = false;
bool      Air::fluid                                = false;
bool      Air::trans                                = true;
BlockType Air::type                                 = BlockType::Air;
u_char    Air::colors[COLOR_DATA_PER_FACE * 6]      = {0}; // TODO: wypełnić
float     Air::texcoords[TEXTURE_DATA_PER_FACE * 6] = {0};