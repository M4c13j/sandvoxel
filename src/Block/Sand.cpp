#include "Sand.hpp"

bool      Sand::fluid                                = false;
bool      Sand::solid                                = true;
bool      Sand::trans                                = false;
BlockType Sand::type                                 = BlockType::Sand;
u_char    Sand::colors[COLOR_DATA_PER_FACE * 6]      = {0}; // TODO: wypełnić
float     Sand::texcoords[TEXTURE_DATA_PER_FACE * 6] = {0};