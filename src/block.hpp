#pragma once
#include "raylib.h"

class Block {
public:
    float tx = 0.0f; // position of texture in texture map
    float ty = 0.0f; // Assuming: every face same texture for now
    Vector3 pos; // redundantne
    bool visible = false; // is face visible

    Block();
    Block(int tx, int ty) : tx(tx), ty(ty) {};
    Block(int tx, int ty, Vector3 pos) : tx(tx), ty(ty), pos(pos) {};
};