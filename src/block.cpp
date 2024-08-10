#include "block.hpp"
#include "config.hpp"
#include <cassert>
#include <string.h>
#include <cstdio>
#include <numeric>
#include <algorithm>

// constexpr FaceData FACE_DATA_VOXEL[] = {
//     {{23.0f}, {24.0f,24.0f}, {23.0f,24.0f}, {21}}
// };

// Data for all faces of the cube
// Assumption here: face has side of 1.0f
const float FACE_VERTICES[] = {
    -0.5, -0.5, 0.5,
    0.5, -0.5, 0.5,
    0.5, 0.5, 0.5,
    -0.5, 0.5, 0.5,
    -0.5, -0.5, -0.5,
    -0.5, 0.5, -0.5,
    0.5, 0.5, -0.5,
    0.5, -0.5, -0.5,
    -0.5, 0.5, -0.5,
    -0.5, 0.5, 0.5,
    0.5, 0.5, 0.5,
    0.5, 0.5, -0.5,
    -0.5, -0.5, -0.5,
    0.5, -0.5, -0.5,
    0.5, -0.5, 0.5,
    -0.5, -0.5, 0.5,
    0.5, -0.5, -0.5,
    0.5, 0.5, -0.5,
    0.5, 0.5, 0.5,
    0.5, -0.5, 0.5,
    -0.5, -0.5, -0.5,
    -0.5, -0.5, 0.5,
    -0.5, 0.5, 0.5,
    -0.5, 0.5, -0.5
};

const float FACE_TEXCOORDS[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f
};

const float FACE_NORMALS_RAW[] = {
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f,-1.0f,
    0.0f, 0.0f,-1.0f,
    0.0f, 0.0f,-1.0f,
    0.0f, 0.0f,-1.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f,-1.0f, 0.0f,
    0.0f,-1.0f, 0.0f,
    0.0f,-1.0f, 0.0f,
    0.0f,-1.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f
};

const unsigned short FACE_INDICES[] = {
    0, 1, 2, 0, 2, 3
};


// Modifies dest pointers.
void Block::generate_face(FacePlacementData &dest, Dir dir, Cord pos) {
    assert(dir < 6);

    int off_vert_norm = VERTEX_DATA_PER_FACE * dir; // vert_data * vert_per_face * faces
    int off_texs = TEXTURE_DATA_PER_FACE * dir; // tex_coords * verts_per_face * faces
    // no tak, bo memcpy chce rozmiar struktury, a nie ilośc elementow :(
    std::copy_n(FACE_VERTICES  + off_vert_norm, VERTEX_DATA_PER_FACE,  dest.vertices);
    std::copy_n(FACE_NORMALS_RAW   + off_vert_norm, VERTEX_DATA_PER_FACE,  dest.normals);
    std::copy_n(FACE_TEXCOORDS + off_texs,      TEXTURE_DATA_PER_FACE, dest.texcoords);
    std::copy_n(FACE_INDICES, VERTEX_DATA_PER_FACE / 2, dest.indices);
    for (int i = 0; i < VERTEX_DATA_PER_FACE/3;i++) {
        dest.vertices[3*i + 0] += pos.x;
        dest.vertices[3*i + 1] += pos.y;
        dest.vertices[3*i + 2] += pos.z;
    }
    for(int i = 0; i < 6; i++) {
        dest.indices[i] += dest.indicesOffset;
    }
    dest.indices   += VERTEX_DATA_PER_FACE;
    dest.vertices  += VERTEX_DATA_PER_FACE;
    dest.texcoords += VERTEX_DATA_PER_FACE;
    dest.normals   += VERTEX_DATA_PER_FACE;
}

void Block::draw_face(Cord pos, Dir dir) {
    float *vertices = (float*) RL_MALLOC(VERTEX_DATA_PER_FACE * sizeof(float));
    float *texcoords = (float*) RL_MALLOC(TEXTURE_DATA_PER_FACE* sizeof(float));
    float *normals = (float*) RL_MALLOC(VERTEX_DATA_PER_FACE * sizeof(float));
    unsigned short *indices = (unsigned short *)RL_MALLOC(VERTEX_DATA_PER_FACE / 2  * sizeof(unsigned short));

    Mesh faceMesh = { 0 };

    FacePlacementData ptrs = (FacePlacementData){vertices, texcoords, normals, indices, 0};
    generate_face(ptrs, dir, pos);
    faceMesh.vertices = vertices;
    faceMesh.texcoords = texcoords;
    faceMesh.normals = normals;
    faceMesh.indices = indices;
    faceMesh.triangleCount = 2; // change it
    faceMesh.vertexCount = 4;
    UploadMesh(&faceMesh, false);

    static const Texture bt = LoadTexture("../resources/textures/dirt_plank.png");
    Model model = LoadModelFromMesh(faceMesh);
    model.materials[0] = LoadMaterialDefault();
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = bt;
    // DEBUG ========
    // printf("[Face drawn] dir: %d, pos: {%f, %f, %f}, \n",
    //     dir, pos.x, pos.y, pos.z);
    for (int i = 0; i < 4; i++) {
        DrawSphere({vertices[3*i], vertices[3*i+1], vertices[3*i+2]}, 0.1, GREEN);
        printf("\tp%d: %f %f %fi\n", i, vertices[3*i], vertices[3*i+1], vertices[3*i+2]);
    }
    DrawBoundingBox(GetModelBoundingBox(model), PURPLE);
    // DEBUG ========

    DrawModel(model, {0,0,0}, 1, WHITE);

    UnloadModel(model);
}