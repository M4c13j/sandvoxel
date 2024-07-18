#pragma once
#include "raylib.h"
class Player {
    public:
        Camera camera; // Information about pos, fov etc.

        Player(); // default constructor
        Player(Vector3 pos, Vector3 target, Vector3 up, float fov, int projection);
        ~Player();
};