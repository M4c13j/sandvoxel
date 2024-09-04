#pragma once
#include "raylib.h"

#include <string>

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

extern const struct Cord FACE_NORMALS_CORD[];

struct FacePlacementData {
    unsigned short  indicesOffset;
    float          *vertices;
    float          *texcoords;
    float          *normals;
    unsigned short *indices;
    unsigned char  *colors;

    void advance_face() {
        vertices += VERTEX_DATA_PER_FACE;
        texcoords += TEXTURE_DATA_PER_FACE;
        normals += VERTEX_DATA_PER_FACE;
        indices += INDICES_DATA_PER_FACE;
        colors += COLOR_DATA_PER_FACE;
        indicesOffset += 4; // 4 vertices used for drawing
    }
};



struct Cord {
    int x, y, z;

    Cord(const int x = 0, const int y = 0, const int z = 0) : x(x), y(y), z(z){};

    static Cord fromVec3(const Vector3 vec) {
        return {static_cast<int>(vec.x), static_cast<int>(vec.y), static_cast<int>(vec.z)};
    }
    explicit operator Vector3() const {
        return Vector3{static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)};
    }

    bool operator==(const Cord &rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
    bool operator!=(const Cord &rhs) const { return !(*this == rhs); }

    static std::size_t hash_value(const Cord &obj) {
        // generated by intellij, I didn even read it dawg wtf
        std::size_t seed = 0x28621F59;
        seed ^= (seed << 6) + (seed >> 2) + 0x67F9A5DC + static_cast<std::size_t>(obj.x);
        seed ^= (seed << 6) + (seed >> 2) + 0x355E2863 + static_cast<std::size_t>(obj.y);
        seed ^= (seed << 6) + (seed >> 2) + 0x5B06034C + static_cast<std::size_t>(obj.z);
        return seed;
    }
    friend bool operator<(const Cord &lhs, const Cord &rhs) {
        if (lhs.x < rhs.x)
            return true;
        if (rhs.x < lhs.x)
            return false;
        if (lhs.y < rhs.y)
            return true;
        if (rhs.y < lhs.y)
            return false;
        return lhs.z < rhs.z;
    }
    friend bool operator<=(const Cord &lhs, const Cord &rhs) { return !(rhs < lhs); }
    friend bool operator>(const Cord &lhs, const Cord &rhs) { return rhs < lhs; }
    friend bool operator>=(const Cord &lhs, const Cord &rhs) { return !(lhs < rhs); }
    Cord operator+(const Cord &rhs) const { return (Cord){x + rhs.x, y + rhs.y, z + rhs.z}; }
    Cord operator-(const Cord &rhs) const { return (Cord){x - rhs.x, y - rhs.y, z - rhs.z}; }
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
    void add_dir(Dir dir) {
        x += FACE_NORMALS_CORD[dir].x;
        y += FACE_NORMALS_CORD[dir].y, z += FACE_NORMALS_CORD[dir].z;
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


// https://stackoverflow.com/questions/50888127/how-can-i-use-an-unordered-set-with-a-custom-struct
template <> struct std::hash<Cord> {
    std::size_t operator()(const Cord &obj) const noexcept {
        // generated by intellij, I didn even read it dawg wtf
        size_t seed = 0x28621F58;
        seed ^= (seed << 5) + (seed >> 2) + 0x67F9A5DC + static_cast<std::size_t>(obj.x);
        seed ^= (seed << 5) + (seed >> 2) + 0x355E2863 + static_cast<std::size_t>(obj.y);
        seed ^= (seed << 5) + (seed >> 2) + 0x5B06034C + static_cast<std::size_t>(obj.z);
        return seed;
    }
};
//============================== BLOCK FACTORY =================================================
// Factory produces blocks. Blocks have texture cordinates and vertex colors assigned to them.
// Enum specifes available block types.

enum class BlockType { Air = 0, Dirt, Plank, Sand, Grass, Fluid, TypeCount };

// Alignment changed size of Block from 40B to 24B, just reordering shit around. Why compilers dont do it?
// Block will of course be optimised in future, it has many redundant stuff, but for now it is what it is.
// Problem: I want isTransparent, type etc. to be static members of actual Block i.e. Sand block.
// That's because I don't need to store copies of colors, texcoords in every single block (which are the same for every
// isntance of block) but I want to be able to change state values like visible.
// Structure:
// Attributes are retrieved through getters.
class Block {
public:
    uint8_t visible = 0; // indexed with index of dir ** 2.
    // static bool      isTransparent;
    // static BlockType type;
    // static u_char    colors[COLOR_DATA_PER_FACE * 6];
    // static float texcoords[TEXTURE_DATA_PER_FACE * 6]; // texture data for every face (indexed by normal from given

    virtual ~Block();
    // virtual ~Block() = default;
    // Block() = default;

    void                generate_face(FacePlacementData &dest, Dir dir, Cord pos);
    [[deprecated]] void draw_face(Cord pos, Dir dir); // used for debuging, long time ago
    virtual bool        isTransparent() = 0;
    virtual bool        isSolid()       = 0;
    virtual bool        isFluid()       = 0;
    virtual BlockType   getType()       = 0;
    virtual u_char     *getColors()     = 0;
    virtual float      *getTexcoords()  = 0;
    virtual void        init()          = 0;

protected:
    Block() = default;
};
