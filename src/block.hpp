#pragma once
#include <vector>
#include "raylib.h"
#include "config.hpp"

const int VERTEX_DATA_PER_FACE = 3 * 4; // for vertices and normals
const int TEXTURE_DATA_PER_FACE = 2 * 4;
const int INDICES_DATA_PER_FACE = 6;

enum Dir {
    DIR_SOUTH = 0,
    DIR_NORTH,
    DIR_UP,
    DIR_DOWN,
    DIR_EAST,
    DIR_WEST,
    COUNT_DIR
};

const Vector3 FACE_NORMALS[] = {
    {0.0f, 0.0f, 1.0f},
    {0.0f, 0.0f,-1.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f,-1.0f, 0.0f},
    {1.0f, 0.0f, 0.0f},
    {-1.0f, 0.0f, 0.0f}
};


struct FacePlacementData {
    float *vertices;
    float *texcoords;
    float *normals;
    unsigned short *indices;
    unsigned short indicesOffset;

    void advance() {
        vertices += VERTEX_DATA_PER_FACE;
        texcoords += TEXTURE_DATA_PER_FACE;
        normals += VERTEX_DATA_PER_FACE;
        indices += INDICES_DATA_PER_FACE;
        indicesOffset += INDICES_DATA_PER_FACE;
    }
};

struct Cord {
    int x, y, z;

    Cord(int x = 0, int y = 0, int z = 0) : x(x), y(y), z(z) {};

    Cord(Vector3 vec) {
        x = vec.x; y = vec.y; z = vec.z;
    }

    operator Vector3() const {
        return Vector3{(float)x, (float)y, (float)z};
    }

    Vector3 toVec3() const {
        return Vector3{(float)x, (float)y, (float)z};
    }

    Cord operator+(const Cord& rhs) const {
        return Cord(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    void scale(int t) {
        x *= t; y *= t; z *= t;
    }
};

// Direction of face (normal)


class Block {
public:
    float tx = 0.0f; // position of texture in texture map
    float ty = 0.0f; // Assuming: every face same texture for now
    Cord pos;
    bool visible = false; // is face visible
    bool visible[6];
    enum Type {
        Air,
        Dirt,
        Plank,
        DirtPlank,
        TypeCount
    } type;

    Block() {};
    Block(Type type) : type(type) {};
    Block(Type type, int tx, int ty) : type(type), tx(tx), ty(ty) {};
    Block(Type type, int tx, int ty, Cord pos) : type(type), tx(tx), ty(ty), pos(pos) {};
    void generate_face(FacePlacementData &dest, Dir dir, Cord pos);
    void draw_face(Cord pos, Dir dir);
};