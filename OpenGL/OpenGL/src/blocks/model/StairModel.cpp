#include <algorithm>

#include "StairModel.hpp"
#include "blocks/Stair_Block.hpp"
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

StairModel::StairModel()
{}

StairModel* StairModel::get() {
  static StairModel inst;
  return &inst;
}

static const std::array<std::array<size_t, 7>, 4> facingLookup {
  // TOP, BOTTOM, FRONT, RIGHT, BACK, LEFT, INNER
  std::array<size_t, 7>
  { 0, 1, 2, 3, 4, 5, 6 }, // NORTH
  { 0, 1, 4, 5, 2, 3, 6 }, // SOUTH
  { 0, 1, 3, 4, 5, 2, 6 }, // EAST
  { 0, 1, 5, 2, 3, 4, 6 }, // WEST
};

void StairModel::genFace(glm::ivec3 pos, BlockFace face, Stair_Block* block, std::array<Block*, 26> const& neighbors, MeshData& data) const {
    auto& _ind = data.indicesSolid;
    auto& _scheme = data.scheme;
    auto& _pos  = data.positions;
    auto& _norm = data.normals;
    auto& _uvs  = data.textureCoords;
    auto& _occl = data.occlusion;
    auto& _normm = data.normalMapCoords;

    Facing facing = block->getFacing();
    size_t flookup = facingLookup[(size_t)facing][(size_t)face];

    // indices
    size_t quadCount = mesh[flookup].size();
    for(size_t i = 0; i < quadCount; i++) {
      _ind.insert(_ind.end(), _scheme.begin(), _scheme.end());
      std::transform(_scheme.begin(), _scheme.end(), _scheme.begin(), [](int x) { return x+4; });
    }

    // positions
    auto& posFace = blockPositions[(size_t)facing][flookup];
    _pos.insert(_pos.end(), posFace.begin(), posFace.end());
    for(int i = 0, k = 0; i < posFace.size(); i++, k = (k+1) % 3) {
      _pos[_pos.size() - posFace.size() + i] += pos[k];
    }

    // normals
    if(face == BlockFace::INNER) {
      auto const& normFace = blockInnerNormals[(size_t)facing];
      _norm.insert(_norm.end(), normFace.begin(), normFace.end());
    }
    else {
      auto const& normFace = blockNormals[flookup];
      for(size_t i = 0; i < quadCount; i++) {
        _norm.insert(_norm.end(), normFace.begin(), normFace.end());
      }
    }

    // textureCoords
    auto indexUV = block->getFaceUVs(face);
    auto uvFace = computeUV(indexUV, UVMesh[flookup]);
    _uvs.insert(_uvs.end(), uvFace.begin(), uvFace.end());

    // occlusion
    if(face == BlockFace::INNER) {
      auto const& occl = blockInnerOcclusions[(size_t)facing];
      _occl.insert(_occl.end(), occl.begin(), occl.end());
    }
    else {
      auto occl = genOcclusion(pos, neighbors, face);
      _occl.insert(_occl.end(), occl.begin(), occl.end());
      for(size_t i = 0; i < quadCount - 1; i++) {
        static const std::vector<GLfloat> empty(4);
        _occl.insert(_occl.end(), empty.begin(), empty.end());
      }
    }

    // normalMapCoords
    for(size_t i = 0; i < quadCount; i++) {
      _normm.insert(_normm.end(), faceNormalMap.begin(), faceNormalMap.end());
    }
}

void StairModel::generateMesh(ivec3 pos, Block* block, std::array<Block*, 26> const& neighbors, MeshData& data) const {
  auto stair = dynamic_cast<Stair_Block*>(block);

  for(auto const& off : blockFaceOffsets) {
    auto neigh = neighbors[off.first];
    if(!neigh->isOpaque()) {
      genFace(pos, off.second, stair, neighbors, data);
    }
  }

  genFace(pos, BlockFace::INNER, stair, neighbors, data);
}

/// below is all the data and lookup tables

const QuadMesh<3> StairModel::mesh = {
  std::vector<Quad<3>> { // TOP
    { vec3
      { -0.5f,  0.5f,  0.0f },
      {  0.5f,  0.5f,  0.0f },
      {  0.5f,  0.5f, -0.5f },
      { -0.5f,  0.5f, -0.5f },
    }
  }, { // BOTTOM
    { vec3
      {  0.5f, -0.5f,  0.5f },
      { -0.5f, -0.5f,  0.5f },
      { -0.5f, -0.5f, -0.5f },
      {  0.5f, -0.5f, -0.5f },
    }
  }, { // FRONT
    { vec3
      {  0.5f,  0.0f,  0.5f },
      { -0.5f,  0.0f,  0.5f },
      { -0.5f, -0.5f,  0.5f },
      {  0.5f, -0.5f,  0.5f },
    }
  }, { // RIGHT
    { vec3 // top
      { 0.5f,  0.5f, -0.5f },
      { 0.5f,  0.5f,  0.0f },
      { 0.5f,  0.0f,  0.0f },
      { 0.5f,  0.0f, -0.5f },
    }, { vec3 // bottom
      { 0.5f,  0.0f, -0.5f },
      { 0.5f,  0.0f,  0.5f },
      { 0.5f, -0.5f,  0.5f },
      { 0.5f, -0.5f, -0.5f },
    }
  }, { // BACK
    { vec3
      { -0.5f,  0.5f, -0.5f },
      {  0.5f,  0.5f, -0.5f },
      {  0.5f, -0.5f, -0.5f },
      { -0.5f, -0.5f, -0.5f },
    }
  }, { // LEFT
    { vec3 // top
      { -0.5f,  0.5f,  0.0f },
      { -0.5f,  0.5f, -0.5f },
      { -0.5f,  0.0f, -0.5f },
      { -0.5f,  0.0f,  0.0f },
    }, { vec3 // bottom
      { -0.5f,  0.0f,  0.5f },
      { -0.5f,  0.0f, -0.5f },
      { -0.5f, -0.5f, -0.5f },
      { -0.5f, -0.5f,  0.5f },
    }
  }, { // INNER
    { vec3 // top
      { -0.5f,  0.0f,  0.5f },
      {  0.5f,  0.0f,  0.5f },
      {  0.5f,  0.0f,  0.0f },
      { -0.5f,  0.0f,  0.0f },
    }, { vec3 // front
      {  0.5f,  0.5f,  0.0f },
      { -0.5f,  0.5f,  0.0f },
      { -0.5f,  0.0f,  0.0f },
      {  0.5f,  0.0f,  0.0f },
    }
  }
};

const QuadMesh<2> StairModel::UVMesh = {
  std::vector<Quad<2>> { // TOP
    { vec2
      { 1.0f, 0.5f },
      { 0.0f, 0.5f },
      { 0.0f, 1.0f },
      { 1.0f, 1.0f },
    }
  }, { // BOTTOM
    { vec2
      { 1.0f, 0.5f },
      { 0.0f, 0.5f },
      { 0.0f, 1.0f },
      { 1.0f, 1.0f },
    }
  }, { // FRONT
    { vec2
      { 1.0f, 0.0f },
      { 0.0f, 0.0f },
      { 0.0f, 0.5f },
      { 1.0f, 0.5f },
    }
  }, { // RIGHT
    { vec2 // top
      { 1.0f, 0.0f },
      { 0.5f, 0.0f },
      { 0.5f, 0.5f },
      { 1.0f, 0.5f },
    }, { vec2 // bottom
      { 1.0f, 0.5f },
      { 0.0f, 0.5f },
      { 0.0f, 1.0f },
      { 1.0f, 1.0f },
    }
  }, { // BACK
    { vec2
      { 1.0f, 0.0f },
      { 0.0f, 0.0f },
      { 0.0f, 1.0f },
      { 1.0f, 1.0f },
    }
  }, { // LEFT
    { vec2 // top
      { 0.5f, 0.0f },
      { 0.0f, 0.0f },
      { 0.0f, 0.5f },
      { 0.5f, 0.5f },
    }, { vec2 // bottom
      { 1.0f, 0.5f },
      { 0.0f, 0.5f },
      { 0.0f, 1.0f },
      { 1.0f, 1.0f },
    }
  }, { // INNER
    { vec2 // top
      { 1.0f, 0.0f },
      { 0.0f, 0.0f },
      { 0.0f, 0.5f },
      { 1.0f, 0.5f },
    }, { vec2 // front
      { 1.0f, 0.5f },
      { 0.0f, 0.5f },
      { 0.0f, 1.0f },
      { 1.0f, 1.0f },
    }
  }
};

const std::vector<Quad<1>> StairModel::innerOcclusions = {
  Quad<1>{ // top
    vec1{ 0.0f },
    vec1{ 0.0f },
    vec1{ 2.0f },
    vec1{ 2.0f },
  }, Quad<1>{ // front
    vec1{ 0.0f },
    vec1{ 0.0f },
    vec1{ 2.0f },
    vec1{ 2.0f },
  }
};

const std::vector<Quad<3>> StairModel::innerNormals = {
  Quad<3> { // top
    vec3
    { 0.0f, 1.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f },
  }, { // front
    vec3
    { 0.0f, 0.0f, 1.0f },
    { 0.0f, 0.0f, 1.0f },
    { 0.0f, 0.0f, 1.0f },
    { 0.0f, 0.0f, 1.0f },
  }
};

const std::array<BlockMeshData, 4> StairModel::blockPositions = computeFacing(mesh);
const std::array<std::vector<GLfloat>, 4> StairModel::blockInnerOcclusions = computeFacing(innerOcclusions);
const std::array<std::vector<GLfloat>, 4> StairModel::blockInnerNormals = computeFacing(innerNormals);
