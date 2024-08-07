#include "block.hpp"
#include "config.hpp"
#include <cassert>
#include <string.h>
#include <cstdio>
#include <numeric>
#include <algorithm>

FaceData::FaceData(int verts, int tris) {
    vertices  = std::vector<float>(verts * 3);
    texcoords = std::vector<float>(verts * 2);
    normals = std::vector<float>(verts * 3);
    indices = std::vector<unsigned short>(tris * 3); // should be equal to verts / 3 (verts = tris * 3)
}

Block::Block() {};

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

const float FACE_NORMALS[] = {
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

void Block::generate_face(FacePlacementData &dest, config::Dir dir,
        unsigned short ind_offset, Vector3 pos) {
    assert(dir < 6);

    int off_vert_norm = config::VERTEX_DATA_PER_FACE * dir; // vert_data * vert_per_face * faces
    int off_texs = config::TEXTURE_DATA_PER_FACE * dir; // tex_coords * verts_per_face * faces
    // no tak, bo memcpy chce rozmiar struktury, a nie ilośc elementow :(
    std::copy_n(FACE_VERTICES  + off_vert_norm, config::VERTEX_DATA_PER_FACE,  dest.vertices);
    std::copy_n(FACE_NORMALS   + off_vert_norm, config::VERTEX_DATA_PER_FACE,  dest.normals);
    std::copy_n(FACE_TEXCOORDS + off_texs,      config::TEXTURE_DATA_PER_FACE, dest.texcoords);
    std::copy_n(FACE_INDICES, config::VERTEX_DATA_PER_FACE / 2, dest.indices);
    for (int i = 0; i < config::VERTEX_DATA_PER_FACE/3;i++) {
        dest.vertices[3*i + 0] += pos.x;
        dest.vertices[3*i + 1] += pos.y;
        dest.vertices[3*i + 2] += pos.z;
    }
    for(int i = 0; i < 6; i++) {
        dest.indices[i] += ind_offset;
    }
}

void Block::draw_face(Vector3 pos, config::Dir dir) {
    float *vertices = (float*) RL_MALLOC(config::VERTEX_DATA_PER_FACE * sizeof(float));
    float *texcoords = (float*) RL_MALLOC(config::TEXTURE_DATA_PER_FACE* sizeof(float));
    float *normals = (float*) RL_MALLOC(config::VERTEX_DATA_PER_FACE * sizeof(float));
    unsigned short *indices = (unsigned short *)RL_MALLOC(config::VERTEX_DATA_PER_FACE / 2  * sizeof(unsigned short));

    Mesh faceMesh = { 0 };

    FacePlacementData ptrs = (FacePlacementData){vertices, texcoords, normals, indices};
    generate_face(ptrs, dir, 0, pos);
    faceMesh.vertices = vertices;
    faceMesh.texcoords = texcoords;
    faceMesh.normals = normals;
    faceMesh.indices = indices;
    faceMesh.triangleCount = 2; // change it
    faceMesh.vertexCount = 4;
    UploadMesh(&faceMesh, false);

    // static Material material = LoadMaterialDefault();
    // material.maps[MATERIAL_MAP_DIFFUSE].texture = LoadDefa;
    static const Texture bt = LoadTexture("../resources/textures/dirt_plank.png");
    Model model = LoadModelFromMesh(faceMesh);
    model.materials[0] = LoadMaterialDefault();
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = bt;

    // DEBUG: it may need to be uncommented
    // SetModelMeshMaterial(&model, 0, 0);
    // DrawModelEx(model, {0, 0, 0}, (Vector3){0, 0, 0},
    //             0, (Vector3){1, 1, 1}, WHITE);

    printf("[Face drawn] dir: %d, pos: {%f, %f, %f}, \n",
        dir, pos.x, pos.y, pos.z);

    for (int i = 0; i < 4; i++) {
        DrawSphere({vertices[3*i], vertices[3*i+1], vertices[3*i+2]}, 0.1, GREEN);
        printf("\tp%d: %f %f %fi\n", i, vertices[3*i], vertices[3*i+1], vertices[3*i+2]);
    }
    DrawModel(model, {0,0,0}, 1, WHITE);
    DrawBoundingBox(GetModelBoundingBox(model), PURPLE);

    UnloadModel(model);
}