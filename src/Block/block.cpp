#include "block.hpp"
#include <cassert>
#include <string.h>
#include <cstdio>
#include <numeric>
#include <algorithm>


// Data for all faces of the cube
// Assumption here: face has side of 1.0f and cordinates of cube are in the middle (not in any vertex)
static const float BLOCK_VERTICES[] = {
    0.0, 0.0, 1.0,
    1.0, 0.0, 1.0,
    1.0, 1.0, 1.0,
    0.0, 1.0, 1.0,
    0.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    1.0, 1.0, 0.0,
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 1.0,
    1.0, 1.0, 1.0,
    1.0, 1.0, 0.0,
    0.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    1.0, 0.0, 0.0,
    1.0, 1.0, 0.0,
    1.0, 1.0, 1.0,
    1.0, 0.0, 1.0,
    0.0, 0.0, 0.0,
    0.0, 0.0, 1.0,
    0.0, 1.0, 1.0,
    0.0, 1.0, 0.0
};

static const float BLOCK_TEXCOORDS[] = {
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

static const float FACE_NORMALS_FLAT[] = {
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

static const float g_color_buffer_data[] = {
    0.583f,  0.771f,  0.014f,
    0.609f,  0.115f,  0.436f,
    0.327f,  0.483f,  0.844f,
    0.822f,  0.569f,  0.201f,
    0.435f,  0.602f,  0.223f,
    0.310f,  0.747f,  0.185f,
    0.597f,  0.770f,  0.761f,
    0.559f,  0.436f,  0.730f,
    0.359f,  0.583f,  0.152f,
    0.483f,  0.596f,  0.789f,
    0.559f,  0.861f,  0.639f,
    0.195f,  0.548f,  0.859f,
    0.014f,  0.184f,  0.576f,
    0.771f,  0.328f,  0.970f,
    0.406f,  0.615f,  0.116f,
    0.676f,  0.977f,  0.133f,
    0.971f,  0.572f,  0.833f,
    0.140f,  0.616f,  0.489f,
    0.997f,  0.513f,  0.064f,
    0.945f,  0.719f,  0.592f,
    0.543f,  0.021f,  0.978f,
    0.279f,  0.317f,  0.505f,
    0.167f,  0.620f,  0.077f,
    0.347f,  0.857f,  0.137f,
    0.055f,  0.953f,  0.042f,
    0.714f,  0.505f,  0.345f,
    0.783f,  0.290f,  0.734f,
    0.722f,  0.645f,  0.174f,
    0.302f,  0.455f,  0.848f,
    0.225f,  0.587f,  0.040f,
    0.517f,  0.713f,  0.338f,
    0.053f,  0.959f,  0.120f,
    0.393f,  0.621f,  0.362f,
    0.673f,  0.211f,  0.457f,
    0.820f,  0.883f,  0.371f,
    0.982f,  0.099f,  0.879f
};

/// Order in which vertices should be drawn to make triangles for face.
/// It includes orientation for raylib so that texture is drawn in proper direction.
const unsigned short FACE_INDICES[] = {
    0, 1, 2, 0, 2, 3
};


// Modifies dest pointers.
void Block::generate_face(FacePlacementData &dest, Dir dir, Cord pos) {
    const int off_vert_norm = VERTEX_DATA_PER_FACE * dir;  // vert_data * vert_per_face * faces
    const int off_texs      = TEXTURE_DATA_PER_FACE * dir; // tex_coords * verts_per_face * faces
    const int off_cols = COLOR_DATA_PER_FACE * dir;
    // Blocktype independent
    std::copy_n(BLOCK_VERTICES + off_vert_norm, VERTEX_DATA_PER_FACE, dest.vertices);
    for (int i = 0; i < VERTEX_DATA_PER_FACE / 3; i++) {
        dest.vertices[3 * i + 0] += pos.x;
        dest.vertices[3 * i + 1] += pos.y;
        dest.vertices[3 * i + 2] += pos.z;
    }
    std::copy_n(FACE_NORMALS_FLAT + off_vert_norm, VERTEX_DATA_PER_FACE, dest.normals);
    std::copy_n(FACE_INDICES, VERTEX_DATA_PER_FACE / 2, dest.indices);
    for (int i = 0; i < 6; i++) {
        dest.indices[i] += dest.indicesOffset;
    }
    // Block-type dependent
    std::copy_n(texcoords + off_texs, TEXTURE_DATA_PER_FACE, dest.texcoords);
    std::copy_n(colors + off_cols, COLOR_DATA_PER_FACE, dest.colors);
}

void Block::draw_face(Cord pos, Dir dir) {
    // float *vertices = (float*) RL_MALLOC(VERTEX_DATA_PER_FACE * sizeof(float));
    // float *texcoords = (float*) RL_MALLOC(TEXTURE_DATA_PER_FACE* sizeof(float));
    // float *normals = (float*) RL_MALLOC(VERTEX_DATA_PER_FACE * sizeof(float));
    // unsigned short *indices = (unsigned short *)RL_MALLOC(VERTEX_DATA_PER_FACE / 2  * sizeof(unsigned short));
    //
    // Mesh faceMesh = { 0 };
    //
    // FacePlacementData ptrs = (FacePlacementData){vertices, texcoords, normals, indices, 0};
    // generate_face(ptrs, dir, pos);
    // faceMesh.vertices = vertices;
    // faceMesh.texcoords = texcoords;
    // faceMesh.normals = normals;
    // faceMesh.indices = indices;
    // faceMesh.triangleCount = 2; // change it
    // faceMesh.vertexCount = 4;
    // UploadMesh(&faceMesh, false);
    //
    // static const Texture bt = LoadTexture("../resources/textures/dirt_plank.png");
    // Model model = LoadModelFromMesh(faceMesh);
    // model.materials[0] = LoadMaterialDefault();
    // model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = bt;
    // // DEBUG ========
    // // printf("[Face drawn] dir: %d, pos: {%f, %f, %f}, \n",
    // //     dir, pos.x, pos.y, pos.z);
    // for (int i = 0; i < 4; i++) {
    //     DrawSphere({vertices[3*i], vertices[3*i+1], vertices[3*i+2]}, 0.1, GREEN);
    //     printf("\tp%d: %f %f %fi\n", i, vertices[3*i], vertices[3*i+1], vertices[3*i+2]);
    // }
    // DrawBoundingBox(GetModelBoundingBox(model), PURPLE);
    // // DEBUG ========
    //
    // DrawModel(model, {0,0,0}, 1, WHITE);
    //
    // UnloadModel(model);
}
