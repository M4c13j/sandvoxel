#include "block.hpp"

FaceData::FaceData(int verts, int tris) {
    vertices  = std::vector<float>(verts * 3);
    texcoords = std::vector<float>(verts * 2);
    normals = std::vector<float>(verts * 3);
    indices = std::vector<unsigned short>(tris * 3); // should be equal to verts / 3 (verts = tris * 3)
}

Block::Block() {};

constexpr FaceData FACE_DATA_VOXEL[] = {
    {{23.0f}, {24.0f,24.0f}, {23.0f,24.0f}, {21}}
};

void Block::generate_face(FacePlacementData &face, config::Directions dir) {

}