#pragma once
#include "config.hpp"
#include "raylib.h"
#include <bitset>

constexpr int VERTEX_DATA_PER_FACE  = 3 * 4; // for vertices and normals
constexpr int TEXTURE_DATA_PER_FACE = 2 * 4;
constexpr int INDICES_DATA_PER_FACE = 6;
constexpr int COLOR_DATA_PER_FACE   = 4 * 4; // RGBA uchar per every Vertex

// Z Y X  + -    (order of directions)
enum Dir {
    DIR_NORTH = 0,
    DIR_SOUTH,
    DIR_UP,
    DIR_DOWN,
    DIR_EAST,
    DIR_WEST,
    DIR_COUNT
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

    void advance_face() {
        vertices += VERTEX_DATA_PER_FACE;
        texcoords += TEXTURE_DATA_PER_FACE;
        normals += VERTEX_DATA_PER_FACE;
        indices += INDICES_DATA_PER_FACE;
        indicesOffset += 4; // 4 vertices used for drawing
    }
};

struct Cord {
    int x, y, z;

    Cord(const int x = 0, const int y = 0, const int z = 0) : x(x), y(y), z(z){};

    Cord(const Vector3 vec) {
        x = vec.x;
        y = vec.y;
        z = vec.z;
    }

    explicit operator Vector3() const {
        return Vector3{static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)};
    }
    Cord operator+(const Cord &rhs) const { return {x + rhs.x, y + rhs.y, z + rhs.z}; }
    Cord operator-(const Cord &rhs) const { return {x - rhs.x, y - rhs.y, z - rhs.z}; }
    void operator+=(const Cord &rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
    }
    void shift(const int d) {
        x -= d;
        y -= d;
        z -= d;
    }
    void scale(const int t) {
        x *= t;
        y *= t;
        z *= t;
    }
    std::string toString() const {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
    }
};

// Alignment changed size of Block from 40B to 24B, just reordering shit around. Why compilers dont do it?
// Block will of course be optimised in future, it has many redundant stuff, but for now it is what it is.
class Block {
public:
    enum Type { Air, Dirt, Plank, DirtPlank, TypeCount } type;
    uint8_t visible = 0; // indexed with index of dir ** 2.

             Block() = default;
    explicit Block(Type type) : type(type){};
    // Block(Type type) : type(type) {};
    ~                  Block() = default;
    void               setType(const Type newType) { type = newType; }
    [[nodiscard]] bool is_transparent() const { return type == Air; }
    void               generate_face(FacePlacementData &dest, Dir dir, Cord pos);
    void               draw_face(Cord pos, Dir dir);
};

inline Dir inverse_dir(Dir dir) {
    // auto val = [](int x) {return !(x%2) + x - x%2;};
    // for (int i = 0; i < 3; i++)
    //     assert(val(2*i) == 2*i+1 && val(2*i+1) == 2*i);
    return static_cast<Dir>(!(dir % 2) + dir - dir % 2);
}