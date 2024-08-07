#pragma once
#include <vector>
#include "raylib.h"
#include "config.hpp"

struct FacePlacementData {
    float *vertices;
    float *texcoords;
    float *normals;
    unsigned short *indices;
};

struct FaceData {
    std::vector<float> vertices;
    std::vector<float> texcoords;
    std::vector<float> normals;
    std::vector<unsigned short> indices;

    FaceData(int verts, int tris);
};

class Block {
public:
    float tx = 0.0f; // position of texture in texture map
    float ty = 0.0f; // Assuming: every face same texture for now
    Vector3 pos; // redundantne
    bool visible = false; // is face visible
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
    Block(Type type, int tx, int ty, Vector3 pos) : type(type), tx(tx), ty(ty), pos(pos) {};
    void generate_face(FacePlacementData &dest, config::Dir dir,
        unsigned short ind_offset, Vector3 pos);
    void draw_face(Vector3 pos, config::Dir dir);
};

extern const Vector3 FACE_NORMALS[6]; // normals by dir