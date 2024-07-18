#include "player.hpp"

Player::Player() {
    camera = (Camera) {
        .position = (Vector3){ 2.0f, 2.0f, 3.0f },    // Camera position
        .target = (Vector3){ 0.0f, 0.5f, 0.0f },      // Camera looking at point
        .up = (Vector3){ 0.0f, 1.0f, 0.0f },          // Camera up vector (rotation towards target)
        .fovy = 90.0f,                                // Camera field-of-view Y
        .projection = CAMERA_PERSPECTIVE              // Camera projection type
    };
}

Player::Player(Vector3 pos, Vector3 target, Vector3 up, float fov, int projection) {
    camera = (Camera) {
        .position = pos,
        .target = target,
        .up = up,
        .fovy = fov,
        .projection = projection
    };
}

Player::~Player() {}